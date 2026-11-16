/**
 *
 * I2C example using a Streamline Storage EEPROM blade (1-4 x M24M01 on I2C1, PB6/PB7).
 *
 * At startup the I2C driver frees the bus if a slave was left stuck mid-transfer (e.g. by a
 * reset in the middle of a read) and the result is printed. The bus runs at 1 MHz, which
 * is out of spec for the STM32F4 but well within the M24M01 rating (see lib/i2c.c).
 * The bus is scanned and the result printed as a table. Then:
 *  - a boot counter stored at the start of the first EEPROM is read, incremented and
 *    written back, so it survives resets and power cycles
 *  - a full 256 byte page is filled with random data (from the RNG), written, read back
 *    and verified
 *
 * After that the blue LED on PC13 blinks to show the program is alive.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "main.h" // Main header contains config and common includes

#include <stdio.h>
#include <string.h>

const uint16_t led = PIN('C', 13);    // Blue LED
const uint16_t i2c_scl = PIN('B', 6); // I2C1 SCL (Streamline I2C1)
const uint16_t i2c_sda = PIN('B', 7); // I2C1 SDA (Streamline I2C1)

#define I2C_SPEED I2C_SPEED_FAST_PLUS // 1 MHz - out of spec for the F4, but the M24M01 handles it

// M24M01: 128 kB, 256 byte pages, 2 byte memory address. Bit 16 of the memory address
// goes in the LSB of the device address, so each chip answers on two addresses.
#define EEPROM_ADDR 0x50         // First chip (E1 = E2 = 0)
#define EEPROM_PAGE_SIZE 256     // Bytes per page - a write must not cross a page boundary
#define EEPROM_WRITE_TIME_MS 10  // Max write cycle time is 5 ms - allow some margin
#define EEPROM_BOOT_COUNT 0x0000 // Location of the boot counter (magic + count)
#define EEPROM_BOOT_MAGIC 0xB007C0DE // Marks a valid boot counter
#define EEPROM_TEST_PAGE 0x0100  // Location of the page used for the write/verify test

/**
 * Scan the bus and print a table of responding addresses (like i2cdetect).
 */
static void i2c_scan(volatile struct i2c *i2c) {
    printf("Scanning I2C bus:\n");
    printf("    ");
    for (int col = 0; col < 16; ++col)
        printf(" %x ", col);
    printf("\n");

    for (int row = 0; row < 8; ++row) {
        printf("%02x: ", row << 4);
        for (int col = 0; col < 16; ++col) {
            uint8_t addr = (uint8_t)((row << 4) | col);
            if (addr < 0x08 || addr > 0x77) // Reserved addresses
                printf("   ");
            else if (i2c_probe(i2c, addr))
                printf("%02x ", addr);
            else
                printf("-- ");
        }
        printf("\n");
    }
}

/**
 * Device address for a given memory address on the first chip.
 */
static uint8_t eeprom_dev(uint32_t mem_addr) {
    return (uint8_t)(EEPROM_ADDR | ((mem_addr >> 16) & 1));
}

/**
 * Wait for the EEPROM to finish its internal write cycle. While busy it does not
 * acknowledge its address ("acknowledge polling").
 */
static int eeprom_wait_ready(uint32_t mem_addr) {
    uint32_t start = s_ticks;

    while (!i2c_probe(I2C1, eeprom_dev(mem_addr))) {
        if (s_ticks - start > EEPROM_WRITE_TIME_MS)
            return I2C_ERR_TIMEOUT;
    }
    return I2C_OK;
}

/**
 * Read any number of bytes - reads can cross pages (but not the 64 kB boundary).
 */
static int eeprom_read(uint32_t mem_addr, uint8_t *data, size_t len) {
    return i2c_read_mem(I2C1, eeprom_dev(mem_addr), mem_addr & 0xFFFF, 2, data, len);
}

/**
 * Write up to one page and wait for the write cycle to complete.
 */
static int eeprom_write(uint32_t mem_addr, const uint8_t *data, size_t len) {
    size_t room = EEPROM_PAGE_SIZE - (mem_addr % EEPROM_PAGE_SIZE);
    int rc;

    if (len > room) // Would wrap around to the start of the page
        return I2C_ERR_PARAM;

    if ((rc = i2c_write_mem(I2C1, eeprom_dev(mem_addr), mem_addr & 0xFFFF, 2, data, len)) != I2C_OK)
        return rc;

    return eeprom_wait_ready(mem_addr);
}

/**
 * Print a buffer as a hex dump, 16 bytes per line.
 */
static void hex_dump(uint32_t addr, const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (i % 16 == 0)
            printf("%s%05lx:", i ? "\n" : "", addr + i);
        printf(" %02x", data[i]);
    }
    printf("\n");
}

/**
 * Read, increment and store the boot counter.
 */
static void boot_count_test(void) {
    struct {
        uint32_t magic; // Marks the counter as valid - anything else (erased or old data) starts over
        uint32_t count;
    } boot;
    int rc;

    if ((rc = eeprom_read(EEPROM_BOOT_COUNT, (uint8_t *)&boot, sizeof(boot))) != I2C_OK) {
        printf("Boot count read failed: %s\n", i2c_strerror(rc));
        return;
    }

    if (boot.magic != EEPROM_BOOT_MAGIC) {
        printf("No valid boot count found - starting from 0\n");
        boot.magic = EEPROM_BOOT_MAGIC;
        boot.count = 0;
    }
    ++boot.count;

    if ((rc = eeprom_write(EEPROM_BOOT_COUNT, (uint8_t *)&boot, sizeof(boot))) != I2C_OK) {
        printf("Boot count write failed: %s\n", i2c_strerror(rc));
        return;
    }

    printf("Boot count: %lu\n", boot.count);
}

/**
 * Write a page of random data, read it back and compare.
 */
static void page_test(void) {
    static uint8_t wr[EEPROM_PAGE_SIZE], rd[EEPROM_PAGE_SIZE];
    uint32_t t_write, t_read;
    int rc;

    for (size_t i = 0; i < sizeof(wr); i += 4) {
        uint32_t r = rng_get_random();
        memcpy(&wr[i], &r, 4);
    }
    memset(rd, 0, sizeof(rd));

    t_write = s_ticks;
    rc = eeprom_write(EEPROM_TEST_PAGE, wr, sizeof(wr));
    t_write = s_ticks - t_write;
    if (rc != I2C_OK) {
        printf("Page write failed: %s\n", i2c_strerror(rc));
        return;
    }

    t_read = s_ticks;
    rc = eeprom_read(EEPROM_TEST_PAGE, rd, sizeof(rd));
    t_read = s_ticks - t_read;
    if (rc != I2C_OK) {
        printf("Page read failed: %s\n", i2c_strerror(rc));
        return;
    }

    size_t errors = 0;
    for (size_t i = 0; i < sizeof(wr); ++i)
        if (wr[i] != rd[i])
            ++errors;

    printf("Page test at 0x%05x: write %lu ms, read %lu ms, %u of %u bytes wrong - %s\n",
           EEPROM_TEST_PAGE, t_write, t_read, errors, sizeof(wr), errors ? "FAILED" : "OK");
    hex_dump(EEPROM_TEST_PAGE, rd, 32);
}

/**
 * Main function - entry point of the program, system initialization
 * and main loop
 */
int main(void) {

    sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz and enable apb1 at 42 MHz, apb2 at 84 MHz

    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode

    uart_init(USART1, 921600); // Initialize USART1 for debugging

    rng_init(); // Random data for the page test

    printf("\n\n\nSystem initialized.\n");
    printf("Core clock  : %9d Hz\n", SYS_FREQUENCY);
    printf("APB1 clock  : %9d Hz\n", APB1_FREQUENCY);
    printf("APB2 clock  : %9d Hz\n", APB2_FREQUENCY);
    printf("I2C speed   : %9d Hz\n", I2C_SPEED);

    // i2c_init() first frees the bus in case a slave was left mid-transfer (e.g. by a
    // reset in the middle of a read) and reports what it found
    int rc = i2c_init(I2C1, i2c_scl, i2c_sda, I2C_SPEED);
    int recovery = i2c_get_last_recovery(I2C1);
    if (recovery == 0)
        printf("I2C bus     : idle\n");
    else if (recovery > 0)
        printf("I2C bus     : was stuck - freed after %d clock(s)\n", recovery);
    else
        printf("I2C bus     : STUCK - %s\n", i2c_strerror(recovery));
    if (rc != I2C_OK)
        printf("I2C init failed: %s\n", i2c_strerror(rc));
    printf("\n");

    i2c_scan(I2C1);
    printf("\n");

    if (i2c_probe(I2C1, EEPROM_ADDR)) {
        boot_count_test();
        page_test();
    } else {
        printf("No EEPROM found at 0x%02x\n", EEPROM_ADDR);
    }

    uint32_t now = 0, next_blink = 0, next_tick = 1000, loop_cnt = 0;

    while (1) { // Super loop

        now = s_ticks;

        if (now >= next_blink) {
            gpio_write(led, !gpio_read(led)); // Toggle LED
            next_blink = now + 500;           // Schedule next toggle in 500 ms
        }

        if (now >= next_tick) {

            // The recovery count shows whether the I2C bus has needed rescuing since startup
            printf("Tick: %7lu ( loop = %lu i2c recoveries = %lu )\n", now / 1000, loop_cnt, i2c_get_recovery_count(I2C1));

            loop_cnt = 0;
            next_tick = now + 1000; // Schedule next tick in 1000 ms
        }

        ++loop_cnt; // Just a counter to show how many times the loop runs between ticks
    }

    return 0;
}

/**
 * Write function for printf
 */
int _write(int fd, char *ptr, int len) {
    int i = 0;

    /*
     * Write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
     * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2) {
        return -1;
    }
    while (*ptr && (i < len)) {

        if (*ptr == '\n') { // Convert newline to carriage return + newline for proper terminal formatting
            uart_write_char(USART1, '\r');
        }

        uart_write_char(USART1, *ptr);

        i++;

        ptr++;
    }
    return i;
}

// vim: et ts=4 nowrap autoindent
