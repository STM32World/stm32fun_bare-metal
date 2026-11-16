/**
 *
 * I2C master driver (polling) for STM32F4xx microcontrollers.
 *
 * The F4 I2C peripheral is driven through a sequence of "events" (EV5, EV6, ...)
 * described in the reference manual (RM0090, section 27.3.3). Several flags are cleared
 * as a side effect of reading SR1 followed by SR2 or DR, so the order of register
 * accesses in this file matters. Receiving 1, 2 or 3+ bytes each needs its own
 * sequence to get the NACK and STOP onto the bus at the right moment.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "i2c.h"

// Timeouts are counted in loop iterations rather than SysTick ticks, because some of the
// waits below happen with interrupts disabled. A wait loop is roughly 8 cycles.
#define I2C_TIMEOUT_LOOPS (SYS_FREQUENCY / 1000 / 8 * I2C_TIMEOUT_MS)

// How long bus recovery waits for SCL to go high after releasing it, in i2c_delay() units
// (~5 us each). A slave may legitimately stretch the clock, so allow ~1 ms before deciding
// that SCL is held low.
#define I2C_RECOVER_STRETCH_DELAYS 200

// Maximum number of clocks bus recovery sends: up to 8 to finish a byte the slave is
// sending, plus 1 for the acknowledge slot (left high = NACK, so the slave stops sending).
#define I2C_RECOVER_MAX_CLOCKS 9

/**
 * Per-bus configuration remembered by i2c_init(). With this the driver can free and
 * re-initialize a bus on its own when a transfer times out, instead of leaving it dead
 * until the next reset.
 */
struct i2c_state {
    bool initialized;   // i2c_init() has been called for this bus
    uint16_t scl;       // SCL pin
    uint16_t sda;       // SDA pin
    uint32_t hz;        // Bus speed
    int last_recovery;  // Result of the most recent i2c_bus_recover() on this bus
    uint32_t recoveries; // Number of times the bus was found stuck (recovery result != 0)
};

static struct i2c_state i2c_states[3]; // I2C1, I2C2, I2C3

/**
 * Look up the state for an I2C instance.
 * @return Pointer to the state, or NULL for an unknown instance
 */
static struct i2c_state *i2c_get_state(volatile struct i2c *i2c) {
    if (i2c == I2C1)
        return &i2c_states[0];
    if (i2c == I2C2)
        return &i2c_states[1];
    if (i2c == I2C3)
        return &i2c_states[2];
    return NULL;
}

/**
 * Disable interrupts and return the previous PRIMASK, so it can be restored.
 * Used around the few steps of a reception where the NACK/STOP must reach the
 * peripheral before the next byte is clocked in.
 * @return Previous PRIMASK value (pass to irq_restore())
 */
static inline uint32_t irq_save(void) {
    uint32_t primask;
    __asm__ volatile("mrs %0, primask\n\tcpsid i" : "=r"(primask)::"memory");
    return primask;
}

/**
 * Restore PRIMASK saved by irq_save(). Interrupts are only re-enabled if they were
 * enabled before, so the pair nests safely.
 * @param primask: Value returned by irq_save()
 */
static inline void irq_restore(uint32_t primask) {
    __asm__ volatile("msr primask, %0" ::"r"(primask) : "memory");
}

/**
 * Busy-wait roughly 5 us (half an SCL period at 100 kHz) for bit-banged bus recovery.
 * The exact length doesn't matter: the master owns the clock, so slower is always safe.
 */
static void i2c_delay(void) {
    for (volatile uint32_t i = SYS_FREQUENCY / 1000000; i > 0; --i)
        (void)0;
}

/**
 * Wait for a flag in SR1, bailing out on NACK, bus errors or timeout.
 * Note that reading SR1 here is part of several clear sequences (e.g. SB, ADDR).
 * @param flag: I2C_SR1_* mask to wait for
 * @return I2C_OK, I2C_ERR_NACK, I2C_ERR_BUS or I2C_ERR_TIMEOUT
 */
static int i2c_wait_flag(volatile struct i2c *i2c, uint32_t flag) {
    uint32_t loops = I2C_TIMEOUT_LOOPS;
    uint32_t sr1;

    while (!((sr1 = i2c->SR1) & flag)) {
        if (sr1 & I2C_SR1_AF)
            return I2C_ERR_NACK;
        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO))
            return I2C_ERR_BUS;
        if (--loops == 0)
            return I2C_ERR_TIMEOUT;
    }
    return I2C_OK;
}

/**
 * Wait for the bus to become idle (no transfer in progress, STOP has been sent).
 * @return I2C_OK, or I2C_ERR_TIMEOUT if BUSY never clears (bus stuck)
 */
static int i2c_wait_idle(volatile struct i2c *i2c) {
    uint32_t loops = I2C_TIMEOUT_LOOPS;

    while ((i2c->SR2 & I2C_SR2_BUSY) || i2c->CR1_b.STOP) {
        if (--loops == 0)
            return I2C_ERR_TIMEOUT;
    }
    return I2C_OK;
}

/**
 * Clear the ADDR flag (EV6) - done by reading SR1 followed by SR2.
 * Clearing ADDR releases SCL and lets the transfer continue, so in receiver mode
 * ACK/POS must be set up BEFORE calling this.
 */
static inline void i2c_clear_addr(volatile struct i2c *i2c) {
    (void)i2c->SR1;
    (void)i2c->SR2;
}

/**
 * Abort a transfer after an error: release the bus and clear the error flags.
 *
 * A NACK is a normal outcome (no device, or an EEPROM busy writing) and just ends the
 * transfer. A timeout or bus error means the bus or the peripheral is in a bad state -
 * e.g. a slave holding SDA low, or a false BUSY latched by the analog filter - so the
 * bus is freed and the peripheral re-initialized with the settings from i2c_init().
 * The original error is still returned, so the caller can retry.
 * @param err: The error that caused the abort
 * @return err, unchanged
 */
static int i2c_abort(volatile struct i2c *i2c, int err) {
    if (i2c->SR2 & I2C_SR2_MSL) // Only a master can (and needs to) send STOP
        i2c->CR1_b.STOP = 1;
    i2c->SR1 = ~I2C_SR1_ERRORS; // Error flags are rc_w0
    i2c->CR1_b.POS = 0;

    if (err == I2C_ERR_TIMEOUT || err == I2C_ERR_BUS) {
        struct i2c_state *st = i2c_get_state(i2c);
        if (st && st->initialized)
            i2c_init(i2c, st->scl, st->sda, st->hz); // Recovers the bus and resets the peripheral
    }
    return err;
}

/**
 * Generate a (repeated) START and send the 7-bit address with the direction bit.
 * Returns with ADDR set but NOT cleared - the caller decides when to clear it.
 * @param addr: 7-bit device address
 * @param read: true for a read (receiver), false for a write (transmitter)
 * @return I2C_OK, or I2C_ERR_NACK if no device answered, or another error
 */
static int i2c_start(volatile struct i2c *i2c, uint8_t addr, bool read) {
    int rc;

    i2c->CR1_b.ACK = 1;   // ACK received bytes by default
    i2c->CR1_b.START = 1; // Generate START
    if ((rc = i2c_wait_flag(i2c, I2C_SR1_SB)) != I2C_OK)
        return rc;

    // EV5: SB is cleared by reading SR1 (done in i2c_wait_flag) followed by writing DR
    i2c->DR = (uint32_t)((addr << 1) | (read ? 1 : 0));

    return i2c_wait_flag(i2c, I2C_SR1_ADDR);
}

/**
 * Send bytes in transmitter mode (ADDR must already be cleared).
 * Returns as soon as the last byte is in DR - wait for BTF before STOP or repeated START.
 * @return I2C_OK, or I2C_ERR_NACK if the device stopped acknowledging, or another error
 */
static int i2c_send(volatile struct i2c *i2c, const uint8_t *data, size_t len) {
    int rc;

    while (len-- > 0) {
        if ((rc = i2c_wait_flag(i2c, I2C_SR1_TXE)) != I2C_OK)
            return rc;
        i2c->DR = *data++;
    }
    return I2C_OK;
}

/**
 * Send the memory address of a register/EEPROM location, most significant byte first.
 * @param mem_addr: The address
 * @param mem_addr_len: Number of address bytes to send (1-4)
 * @return I2C_OK, I2C_ERR_PARAM for a bad length, or a transfer error
 */
static int i2c_send_mem_addr(volatile struct i2c *i2c, uint32_t mem_addr, uint8_t mem_addr_len) {
    uint8_t buf[4];

    if (mem_addr_len > sizeof(buf))
        return I2C_ERR_PARAM;

    for (uint8_t i = 0; i < mem_addr_len; ++i)
        buf[i] = (uint8_t)(mem_addr >> (8 * (mem_addr_len - 1 - i)));

    return i2c_send(i2c, buf, mem_addr_len);
}

/**
 * Receive bytes in receiver mode, then send STOP. Called with ADDR set but not cleared.
 *
 * The master must NACK the last byte and request STOP before that byte has been clocked
 * in, but the peripheral is always one or two bytes ahead (DR + shift register). The
 * 1, 2 and 3+ byte cases therefore follow RM0090 27.3.3 ("Master receiver") exactly:
 *  - 1 byte:  ACK=0 before clearing ADDR, then STOP
 *  - 2 bytes: ACK=0 + POS=1 (NACK applies to the next byte), wait for both, STOP
 *  - 3+:      ACK until 3 bytes remain, then use BTF to stop the clock while
 *             ACK is cleared and STOP is requested
 * @return I2C_OK or an error
 */
static int i2c_receive(volatile struct i2c *i2c, uint8_t *data, size_t len) {
    uint32_t primask;
    int rc;

    if (len == 0) {
        i2c_clear_addr(i2c);
        i2c->CR1_b.STOP = 1;
        return I2C_OK;
    } else if (len == 1) {
        // NACK the only byte and request STOP right after clearing ADDR
        i2c->CR1_b.ACK = 0;
        primask = irq_save();
        i2c_clear_addr(i2c);
        i2c->CR1_b.STOP = 1;
        irq_restore(primask);
    } else if (len == 2) {
        // POS=1 makes the NACK apply to the next byte received (the second one)
        i2c->CR1_b.ACK = 0;
        i2c->CR1_b.POS = 1;
        i2c_clear_addr(i2c);
    } else {
        i2c_clear_addr(i2c); // ACK stays on until the last three bytes
    }

    while (len > 0) {
        if (len > 3) {
            if ((rc = i2c_wait_flag(i2c, I2C_SR1_RXNE)) != I2C_OK)
                return rc;
            *data++ = (uint8_t)i2c->DR;
            --len;
        } else if (len == 3) {
            // Byte N-2 in DR, N-1 in shift register: NACK the last byte
            if ((rc = i2c_wait_flag(i2c, I2C_SR1_BTF)) != I2C_OK)
                return rc;
            i2c->CR1_b.ACK = 0;
            primask = irq_save();
            *data++ = (uint8_t)i2c->DR; // Byte N-2
            if ((rc = i2c_wait_flag(i2c, I2C_SR1_BTF)) != I2C_OK) {
                irq_restore(primask);
                return rc;
            }
            i2c->CR1_b.STOP = 1;
            *data++ = (uint8_t)i2c->DR; // Byte N-1
            irq_restore(primask);
            *data++ = (uint8_t)i2c->DR; // Byte N (already NACKed)
            len = 0;
        } else if (len == 2) {
            // Both bytes received (one in DR, one in shift register)
            if ((rc = i2c_wait_flag(i2c, I2C_SR1_BTF)) != I2C_OK)
                return rc;
            primask = irq_save();
            i2c->CR1_b.STOP = 1;
            *data++ = (uint8_t)i2c->DR;
            irq_restore(primask);
            *data++ = (uint8_t)i2c->DR;
            len = 0;
        } else { // len == 1
            if ((rc = i2c_wait_flag(i2c, I2C_SR1_RXNE)) != I2C_OK)
                return rc;
            *data++ = (uint8_t)i2c->DR;
            len = 0;
        }
    }

    i2c->CR1_b.POS = 0;
    return I2C_OK;
}

/**
 * Release SCL and wait for it to actually go high. A slave may hold SCL low for a while
 * (clock stretching), so give it up to ~1 ms.
 * @return true if SCL is high, false if something keeps holding it low
 */
static bool i2c_release_scl(uint16_t scl) {
    gpio_write(scl, 1);
    for (int i = 0; i < I2C_RECOVER_STRETCH_DELAYS; ++i) {
        if (gpio_read(scl))
            return true;
        i2c_delay();
    }
    return gpio_read(scl);
}

/**
 * Free a stuck bus and reset every slave on it, by bit-banging the pins.
 *
 * The classic hang: the MCU resets (or a transfer is aborted) in the middle of a byte.
 * The slave's state machine is still waiting for the rest of the clocks, and if it was
 * sending (a read) it may be holding SDA low, which blocks every START from the master.
 *
 * The sequence:
 *  1. Release both lines and check SCL. If SCL stays low, a slave is stretching
 *     forever or the line is shorted - clocking can't help, so give up.
 *  2. While SDA is low (sampled with SCL high, when data is valid), send a clock.
 *     A transmitting slave shifts out its next bit on each clock; as soon as it
 *     outputs a 1 (released SDA) we stop. After at most 9 clocks the byte is done and
 *     the acknowledge slot has been left high (NACK), so the slave stops sending.
 *  3. Send START followed by STOP. The START is the important part: slaves recognize
 *     it at any point and reset their state machine. The textbook version ends with a
 *     STOP only, but if the slave was being WRITTEN to (master drives SDA, so it was
 *     never low and no clocks were sent), a STOP can be taken as the end of the write -
 *     on an EEPROM that may program a half-received page. A START abandons it instead.
 *  4. Check that both lines are high (bus idle).
 *
 * Both pins are left as open-drain GPIO outputs (released high). i2c_init() calls this
 * before handing the pins to the peripheral.
 *
 * @param scl: SCL pin, e.g. PIN('B', 6)
 * @param sda: SDA pin, e.g. PIN('B', 7)
 * @return Number of clocks it took to free SDA (0 = the bus was idle), or
 *         I2C_ERR_SCL_LOW / I2C_ERR_SDA_LOW if the bus could not be freed
 */
int i2c_bus_recover(uint16_t scl, uint16_t sda) {
    int clocks = 0;

    // Release both lines (open-drain high) before switching from AF to GPIO output,
    // so the switch itself does not drive anything low
    gpio_write(scl, 1);
    gpio_write(sda, 1);
    gpio_set_otype(scl, GPIO_OTYPE_OPEN_DRAIN);
    gpio_set_otype(sda, GPIO_OTYPE_OPEN_DRAIN);
    gpio_set_pull(scl, GPIO_PULL_UP);
    gpio_set_pull(sda, GPIO_PULL_UP);
    gpio_set_mode(scl, GPIO_MODE_OUTPUT);
    gpio_set_mode(sda, GPIO_MODE_OUTPUT);
    i2c_delay();

    // 1. SCL must be free, or nothing else will work
    if (!i2c_release_scl(scl))
        return I2C_ERR_SCL_LOW;

    // 2. Clock until the slave lets go of SDA (IDR reads the pin even in output mode)
    while (!gpio_read(sda)) {
        if (clocks == I2C_RECOVER_MAX_CLOCKS)
            return I2C_ERR_SDA_LOW; // Not a stuck transfer - SDA shorted or a dead slave
        gpio_write(scl, 0);
        i2c_delay();
        if (!i2c_release_scl(scl))
            return I2C_ERR_SCL_LOW;
        i2c_delay(); // SDA is sampled with SCL high, at the top of the loop
        ++clocks;
    }

    // 3a. START: SDA high -> low while SCL is high. Resets every slave's state machine.
    gpio_write(sda, 0);
    i2c_delay();
    gpio_write(scl, 0);
    i2c_delay();

    // 3b. STOP: SDA low -> high while SCL is high. Leaves the bus idle.
    if (!i2c_release_scl(scl)) {
        gpio_write(sda, 1);
        return I2C_ERR_SCL_LOW;
    }
    i2c_delay();
    gpio_write(sda, 1);
    i2c_delay();

    // 4. Both lines should now be high
    if (!gpio_read(sda))
        return I2C_ERR_SDA_LOW;

    return clocks;
}

/**
 * Initialize an I2C peripheral as master.
 * @param i2c: I2C1, I2C2 or I2C3
 * @param scl: SCL pin, e.g. PIN('B', 6)
 * @param sda: SDA pin, e.g. PIN('B', 7)
 * @param hz: Bus speed - I2C_SPEED_STANDARD, I2C_SPEED_FAST, I2C_SPEED_FAST_PLUS or anything
 *            in between. Above I2C_SPEED_FAST the peripheral runs outside its specification:
 *            it works on a short bus with strong pull-ups (tested on the Streamline EEPROM
 *            blade up to ~1.05 MHz), but there are no guarantees.
 * @return I2C_OK, I2C_ERR_PARAM, or I2C_ERR_SCL_LOW / I2C_ERR_SDA_LOW if the bus is stuck
 *         and could not be freed (the peripheral is still configured, so a later
 *         transfer will retry the recovery when it times out)
 */
int i2c_init(volatile struct i2c *i2c, uint16_t scl, uint16_t sda, uint32_t hz) {

    struct i2c_state *st = i2c_get_state(i2c);

    if (st == NULL || hz == 0 || hz > I2C_SPEED_FAST_PLUS)
        return I2C_ERR_PARAM;

    // Enable the clock
    if (i2c == I2C1) {
        RCC->APB1ENR_b.I2C1EN = 1;
    } else if (i2c == I2C2) {
        RCC->APB1ENR_b.I2C2EN = 1;
    } else if (i2c == I2C3) {
        RCC->APB1ENR_b.I2C3EN = 1;
    }

    // Remember the settings, so the driver can recover the bus on its own later
    st->initialized = true;
    st->scl = scl;
    st->sda = sda;
    st->hz = hz;

    // Take the peripheral off the bus before bit-banging the pins. When called again
    // after a failed transfer it may still be enabled and acting as master, and it would
    // react to the recovery edges (the errata workaround below starts with PE=0 too).
    i2c->CR1 = I2C_CR1_SWRST;
    i2c->CR1 = 0;

    // Get any slave that was left mid-transfer off the bus before handing over the pins
    st->last_recovery = i2c_bus_recover(scl, sda);
    if (st->last_recovery != 0)
        ++st->recoveries;

    // Pins: open-drain alternate function (AF4 for I2C1-3 on the F407), with pull-ups
    // as a fallback in case there are no external ones
    gpio_set_af(scl, 4);
    gpio_set_af(sda, 4);
    gpio_set_speed(scl, GPIO_SPEED_HIGH);
    gpio_set_speed(sda, GPIO_SPEED_HIGH);
    gpio_set_mode(scl, GPIO_MODE_AF);
    gpio_set_mode(sda, GPIO_MODE_AF);

    // Reset the peripheral AFTER the pins are settled. The I2C input sees the pins even
    // while they are plain GPIO, and the bit-banging above can make the analog filter
    // latch a false BUSY (STM32F4 errata: "I2C analog filter may provide wrong value,
    // locking BUSY flag and preventing master mode entry"). A software reset clears it.
    i2c->CR1 = I2C_CR1_SWRST;
    i2c->CR1 = 0;

    // Timing - all I2C peripherals are on APB1
    uint32_t pclk = APB1_FREQUENCY;
    uint32_t freq_mhz = pclk / 1000000;
    uint32_t ccr;

    i2c->CR2 = freq_mhz; // FREQ: APB1 clock in MHz

    if (hz <= I2C_SPEED_STANDARD) {
        // Standard mode: Thigh = Tlow = CCR * Tpclk
        ccr = pclk / (2 * hz);
        if (ccr < 4)
            ccr = 4;                // Minimum allowed in standard mode
        i2c->CCR = ccr;
        i2c->TRISE = freq_mhz + 1; // 1000 ns max rise time
    } else if (hz <= I2C_SPEED_FAST) {
        // Fast mode, DUTY=0: Tlow = 2 * Thigh, so period = 3 * CCR * Tpclk
        ccr = pclk / (3 * hz);
        i2c->CCR = I2C_CCR_FS | ccr;
        i2c->TRISE = freq_mhz * 300 / 1000 + 1; // 300 ns max rise time
    } else {
        // Fast mode plus (out of spec): same as fast mode, but compensate for the time lost
        // to rise time and filtering each period, rounding to the nearest divider
        uint32_t period_ns = 1000000000 / hz - I2C_FAST_PLUS_COMP_NS;
        ccr = (period_ns * freq_mhz + 1500) / 3000;
        if (ccr < 10)
            ccr = 10;                            // Fastest divider that worked in testing
        i2c->CCR = I2C_CCR_FS | ccr;
        i2c->TRISE = freq_mhz * 120 / 1000 + 1; // 120 ns max rise time
    }

    i2c->CR1_b.PE = 1; // Enable peripheral

    return st->last_recovery < 0 ? st->last_recovery : I2C_OK;
}

/**
 * Result of the most recent bus recovery on this bus (run by i2c_init(), and by the
 * driver itself after a timeout or bus error).
 * @return Number of clocks needed (0 = bus was idle), or I2C_ERR_SCL_LOW / I2C_ERR_SDA_LOW
 */
int i2c_get_last_recovery(volatile struct i2c *i2c) {
    struct i2c_state *st = i2c_get_state(i2c);
    return st ? st->last_recovery : I2C_ERR_PARAM;
}

/**
 * Number of times this bus has been found stuck since startup (recovery result != 0).
 * Useful as a health indicator - on a good bus it stays at 0.
 */
uint32_t i2c_get_recovery_count(volatile struct i2c *i2c) {
    struct i2c_state *st = i2c_get_state(i2c);
    return st ? st->recoveries : 0;
}

/**
 * Human readable name for an I2C return code, for debug output.
 * @param rc: Return code from any i2c_* function
 */
const char *i2c_strerror(int rc) {
    switch (rc) {
    case I2C_OK:
        return "ok";
    case I2C_ERR_NACK:
        return "no acknowledge";
    case I2C_ERR_BUS:
        return "bus error / arbitration lost";
    case I2C_ERR_TIMEOUT:
        return "timeout";
    case I2C_ERR_PARAM:
        return "invalid parameter";
    case I2C_ERR_SCL_LOW:
        return "SCL held low";
    case I2C_ERR_SDA_LOW:
        return "SDA held low";
    default:
        return "unknown error";
    }
}

/**
 * Check whether a device acknowledges its address: START, address + write, STOP.
 * Also the way to poll an EEPROM for the end of its write cycle (it does not
 * acknowledge while busy).
 * @param addr: 7-bit address
 * @return true if a device acknowledged
 */
bool i2c_probe(volatile struct i2c *i2c, uint8_t addr) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK) {
        i2c_abort(i2c, rc);
        return false;
    }

    i2c->CR1_b.POS = 0;
    if ((rc = i2c_start(i2c, addr, false)) != I2C_OK) {
        i2c_abort(i2c, rc);
        return false;
    }

    i2c_clear_addr(i2c);
    i2c->CR1_b.STOP = 1;
    return true;
}

/**
 * Write bytes to a device: START, address + write, data, STOP.
 * @param addr: 7-bit address
 * @param data: Bytes to send
 * @param len: Number of bytes
 * @return I2C_OK or an error (I2C_ERR_NACK if the device did not acknowledge)
 */
int i2c_write(volatile struct i2c *i2c, uint8_t addr, const uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return i2c_abort(i2c, rc);

    i2c->CR1_b.POS = 0;
    if ((rc = i2c_start(i2c, addr, false)) != I2C_OK)
        return i2c_abort(i2c, rc);
    i2c_clear_addr(i2c);

    if ((rc = i2c_send(i2c, data, len)) != I2C_OK)
        return i2c_abort(i2c, rc);

    // EV8_2: wait until the last byte is fully shifted out before STOP
    if ((rc = i2c_wait_flag(i2c, I2C_SR1_BTF)) != I2C_OK)
        return i2c_abort(i2c, rc);

    i2c->CR1_b.STOP = 1;
    return I2C_OK;
}

/**
 * Read bytes from a device: START, address + read, data, STOP.
 * @param addr: 7-bit address
 * @param data: Buffer for the received bytes
 * @param len: Number of bytes to read
 * @return I2C_OK or an error (I2C_ERR_NACK if the device did not acknowledge)
 */
int i2c_read(volatile struct i2c *i2c, uint8_t addr, uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return i2c_abort(i2c, rc);

    i2c->CR1_b.POS = 0;
    if ((rc = i2c_start(i2c, addr, true)) != I2C_OK)
        return i2c_abort(i2c, rc);

    if ((rc = i2c_receive(i2c, data, len)) != I2C_OK)
        return i2c_abort(i2c, rc);

    return I2C_OK;
}

/**
 * Write to a register/memory location: START, address, memory address, data, STOP.
 * @param addr: 7-bit device address
 * @param mem_addr: Register or memory address
 * @param mem_addr_len: Number of memory address bytes (1-4), sent MSB first
 * @param data: Bytes to write
 * @param len: Number of bytes
 * @return I2C_OK or an error
 */
int i2c_write_mem(volatile struct i2c *i2c, uint8_t addr, uint32_t mem_addr, uint8_t mem_addr_len, const uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return i2c_abort(i2c, rc);

    i2c->CR1_b.POS = 0;
    if ((rc = i2c_start(i2c, addr, false)) != I2C_OK)
        return i2c_abort(i2c, rc);
    i2c_clear_addr(i2c);

    if ((rc = i2c_send_mem_addr(i2c, mem_addr, mem_addr_len)) != I2C_OK)
        return i2c_abort(i2c, rc);

    if ((rc = i2c_send(i2c, data, len)) != I2C_OK)
        return i2c_abort(i2c, rc);

    if ((rc = i2c_wait_flag(i2c, I2C_SR1_BTF)) != I2C_OK)
        return i2c_abort(i2c, rc);

    i2c->CR1_b.STOP = 1;
    return I2C_OK;
}

/**
 * Read from a register/memory location: START, address, memory address,
 * repeated START, address + read, data, STOP.
 * @param addr: 7-bit device address
 * @param mem_addr: Register or memory address
 * @param mem_addr_len: Number of memory address bytes (1-4), sent MSB first
 * @param data: Buffer for the received bytes
 * @param len: Number of bytes to read
 * @return I2C_OK or an error
 */
int i2c_read_mem(volatile struct i2c *i2c, uint8_t addr, uint32_t mem_addr, uint8_t mem_addr_len, uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return i2c_abort(i2c, rc);

    i2c->CR1_b.POS = 0;
    if ((rc = i2c_start(i2c, addr, false)) != I2C_OK)
        return i2c_abort(i2c, rc);
    i2c_clear_addr(i2c);

    if ((rc = i2c_send_mem_addr(i2c, mem_addr, mem_addr_len)) != I2C_OK)
        return i2c_abort(i2c, rc);

    // Let the memory address finish before the repeated START
    if ((rc = i2c_wait_flag(i2c, I2C_SR1_BTF)) != I2C_OK)
        return i2c_abort(i2c, rc);

    if ((rc = i2c_start(i2c, addr, true)) != I2C_OK)
        return i2c_abort(i2c, rc);

    if ((rc = i2c_receive(i2c, data, len)) != I2C_OK)
        return i2c_abort(i2c, rc);

    return I2C_OK;
}

// vim: et ts=4 nowrap autoindent
