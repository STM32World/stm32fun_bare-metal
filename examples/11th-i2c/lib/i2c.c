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

/**
 * Disable interrupts and return the previous PRIMASK, so it can be restored.
 */
static inline uint32_t irq_save(void) {
    uint32_t primask;
    __asm__ volatile("mrs %0, primask\n\tcpsid i" : "=r"(primask)::"memory");
    return primask;
}

/**
 * Restore PRIMASK saved by irq_save().
 */
static inline void irq_restore(uint32_t primask) {
    __asm__ volatile("msr primask, %0" ::"r"(primask) : "memory");
}

/**
 * Busy-wait roughly 5 us (half an SCL period at 100 kHz) for bit-banged bus recovery.
 */
static void i2c_delay(void) {
    for (volatile uint32_t i = SYS_FREQUENCY / 1000000; i > 0; --i)
        (void)0;
}

/**
 * Wait for a flag in SR1, bailing out on NACK, bus errors or timeout.
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
 */
static inline void i2c_clear_addr(volatile struct i2c *i2c) {
    (void)i2c->SR1;
    (void)i2c->SR2;
}

/**
 * Abort a transfer after an error: release the bus and clear the error flags.
 */
static int i2c_abort(volatile struct i2c *i2c, int err) {
    if (i2c->SR2 & I2C_SR2_MSL) // Only a master can (and needs to) send STOP
        i2c->CR1_b.STOP = 1;
    i2c->SR1 = ~I2C_SR1_ERRORS; // Error flags are rc_w0
    i2c->CR1_b.POS = 0;
    return err;
}

/**
 * Generate a (repeated) START and send the 7-bit address with the direction bit.
 * Returns with ADDR set but NOT cleared - the caller decides when to clear it.
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
 * The 1, 2 and 3+ byte cases follow RM0090 27.3.3 ("Master receiver").
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
 * Free a stuck bus by bit-banging SCL until the slave lets go of SDA, then send STOP.
 *
 * If the MCU resets in the middle of a read, the slave may still be driving SDA low,
 * waiting for clocks to shift out the rest of a byte. Up to 9 clocks finish that byte
 * and the NACK slot, and the STOP puts the slave back into its idle state.
 * Leaves both pins as open-drain GPIO outputs.
 */
void i2c_bus_recover(uint16_t scl, uint16_t sda) {

    gpio_write(scl, 1);
    gpio_write(sda, 1);
    gpio_set_otype(scl, GPIO_OTYPE_OPEN_DRAIN);
    gpio_set_otype(sda, GPIO_OTYPE_OPEN_DRAIN);
    gpio_set_pull(scl, GPIO_PULL_UP);
    gpio_set_pull(sda, GPIO_PULL_UP);
    gpio_set_mode(scl, GPIO_MODE_OUTPUT);
    gpio_set_mode(sda, GPIO_MODE_OUTPUT);
    i2c_delay();

    // Clock until SDA is released (input data register reads the pin even in output mode)
    for (int i = 0; i < 9 && !gpio_read(sda); ++i) {
        gpio_write(scl, 0);
        i2c_delay();
        gpio_write(scl, 1);
        i2c_delay();
    }

    // STOP condition: SDA goes low to high while SCL is high
    gpio_write(scl, 0);
    i2c_delay();
    gpio_write(sda, 0);
    i2c_delay();
    gpio_write(scl, 1);
    i2c_delay();
    gpio_write(sda, 1);
    i2c_delay();
}

/**
 * Initialize an I2C peripheral as master.
 * @param i2c: I2C1, I2C2 or I2C3
 * @param scl: SCL pin, e.g. PIN('B', 6)
 * @param sda: SDA pin, e.g. PIN('B', 7)
 * @param hz: Bus speed, up to 100000 (standard mode) or 400000 (fast mode)
 * @return I2C_OK or I2C_ERR_PARAM
 */
int i2c_init(volatile struct i2c *i2c, uint16_t scl, uint16_t sda, uint32_t hz) {

    if (hz == 0 || hz > 400000)
        return I2C_ERR_PARAM;

    // Enable the clock and pulse the peripheral reset, which also clears a stuck BUSY flag
    if (i2c == I2C1) {
        RCC->APB1ENR_b.I2C1EN = 1;
        RCC->APB1RSTR_b.I2C1RST = 1;
        RCC->APB1RSTR_b.I2C1RST = 0;
    } else if (i2c == I2C2) {
        RCC->APB1ENR_b.I2C2EN = 1;
        RCC->APB1RSTR_b.I2C2RST = 1;
        RCC->APB1RSTR_b.I2C2RST = 0;
    } else if (i2c == I2C3) {
        RCC->APB1ENR_b.I2C3EN = 1;
        RCC->APB1RSTR_b.I2C3RST = 1;
        RCC->APB1RSTR_b.I2C3RST = 0;
    } else {
        return I2C_ERR_PARAM;
    }

    // Get any slave that was left mid-transfer off the bus before handing over the pins
    i2c_bus_recover(scl, sda);

    // Pins: open-drain alternate function (AF4 for I2C1-3 on the F407), with pull-ups
    // as a fallback in case there are no external ones
    gpio_set_af(scl, 4);
    gpio_set_af(sda, 4);
    gpio_set_speed(scl, GPIO_SPEED_HIGH);
    gpio_set_speed(sda, GPIO_SPEED_HIGH);
    gpio_set_mode(scl, GPIO_MODE_AF);
    gpio_set_mode(sda, GPIO_MODE_AF);

    // Timing - all I2C peripherals are on APB1
    uint32_t pclk = APB1_FREQUENCY;
    uint32_t freq_mhz = pclk / 1000000;
    uint32_t ccr;

    i2c->CR1 = 0;        // Peripheral disabled while configuring
    i2c->CR2 = freq_mhz; // FREQ: APB1 clock in MHz

    if (hz <= 100000) {
        // Standard mode: Thigh = Tlow = CCR * Tpclk
        ccr = pclk / (2 * hz);
        if (ccr < 4)
            ccr = 4;                // Minimum allowed in standard mode
        i2c->CCR = ccr;
        i2c->TRISE = freq_mhz + 1; // 1000 ns max rise time
    } else {
        // Fast mode, DUTY=0: Tlow = 2 * Thigh, so period = 3 * CCR * Tpclk
        ccr = pclk / (3 * hz);
        if (ccr < 1)
            ccr = 1;
        i2c->CCR = I2C_CCR_FS | ccr;
        i2c->TRISE = freq_mhz * 300 / 1000 + 1; // 300 ns max rise time
    }

    i2c->CR1_b.PE = 1; // Enable peripheral

    return I2C_OK;
}

/**
 * Check whether a device acknowledges its address.
 * @param addr: 7-bit address
 */
bool i2c_probe(volatile struct i2c *i2c, uint8_t addr) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return false;

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
 * Write bytes to a device.
 * @param addr: 7-bit address
 */
int i2c_write(volatile struct i2c *i2c, uint8_t addr, const uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return rc;

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
 * Read bytes from a device.
 * @param addr: 7-bit address
 */
int i2c_read(volatile struct i2c *i2c, uint8_t addr, uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return rc;

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
 */
int i2c_write_mem(volatile struct i2c *i2c, uint8_t addr, uint32_t mem_addr, uint8_t mem_addr_len, const uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return rc;

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
 */
int i2c_read_mem(volatile struct i2c *i2c, uint8_t addr, uint32_t mem_addr, uint8_t mem_addr_len, uint8_t *data, size_t len) {
    int rc;

    if ((rc = i2c_wait_idle(i2c)) != I2C_OK)
        return rc;

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
