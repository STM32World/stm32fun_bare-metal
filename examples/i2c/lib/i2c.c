/**
 *
 * I2C driver implementation for STM32F407.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "i2c.h"

/**
 * Internal helper to enable the clock and return APB1 frequency.
 */
static uint32_t _i2c_enable_clock(volatile struct i2c *i2c) {
    uintptr_t addr = (uintptr_t)i2c;
    // volatile uint32_t *rcc_apb1enr = (uint32_t *)(0x40023800 + 0x40);

    if (addr == 0x40005400)
        RCC->APB1ENR_b.I2C1EN = 1; // I2C1
    else if (addr == 0x40005800)
        RCC->APB1ENR_b.I2C2EN = 1; // I2C2
    else if (addr == 0x40005C00)
        RCC->APB1ENR_b.I2C3EN = 1; // I2C3

    return APB1_FREQUENCY;
}

void i2c_init(volatile struct i2c *i2c, uint32_t baudrate) {
    uintptr_t addr = (uintptr_t)i2c;
    uint16_t scl, sda;

    // 1. Enable Peripheral Clock and determine Pins
    uint32_t pclk1 = _i2c_enable_clock(i2c);

    if (addr == 0x40005400) { // I2C1
        RCC->AHB1ENR_b.GPIOBEN = 1;
        scl = PIN('B', 6);
        sda = PIN('B', 7);
    } else if (addr == 0x40005800) { // I2C2
        RCC->AHB1ENR_b.GPIOBEN = 1;
        scl = PIN('B', 10);
        sda = PIN('B', 11);
    } else { // I2C3
        RCC->AHB1ENR_b.GPIOAEN = 1;
        RCC->AHB1ENR_b.GPIOCEN = 1;
        scl = PIN('A', 8);
        sda = PIN('C', 9);
    }

    // 2. Configure GPIO Pins
    uint16_t pins[] = { scl, sda };
    for (int i = 0; i < 2; i++) {
        gpio_set_mode(pins[i], GPIO_MODE_AF);
        gpio_set_af(pins[i], 4); // AF4 is I2C for all F407 pins

        // Manual override for I2C requirements (Open-Drain + High Speed)
        volatile struct gpio *gp = GPIO(PINBANK(pins[i]));
        int n = PINNO(pins[i]);
        gp->OTYPER |= (1U << n);        // 1 = Open-Drain (CRITICAL)
        gp->OSPEEDR |= (3U << (n * 2)); // 3 = Very High Speed
    }

    // 3. Software Reset
    i2c->CR1_b.SWRST = 1;
    i2c->CR1_b.SWRST = 0;

    // 4. Set frequency and timing
    uint32_t freq = pclk1 / 1000000;
    i2c->CR2_b.FREQ = (uint8_t)(freq & 0x3FU);

    uint32_t ccr_val = pclk1 / (baudrate * 2);
    if (ccr_val < 4)
        ccr_val = 4;

    i2c->CCR_b.FS = 0;
    i2c->CCR_b.DUTY = 0;
    i2c->CCR_b.CCR_val = (uint16_t)(ccr_val & 0xFFFU);

    i2c->TRISE = (pclk1 / 1000000) + 1;

    // 5. Enable the peripheral
    i2c->CR1_b.PE = 1;
}

void i2c_start(volatile struct i2c *i2c) {
    i2c->CR1_b.ACK = 1;
    i2c->CR1_b.START = 1;
    while (!i2c->SR1_b.SB)
        ;
}

void i2c_stop(volatile struct i2c *i2c) {
    i2c->CR1_b.STOP = 1;
}

void i2c_send_address(volatile struct i2c *i2c, uint8_t addr, bool read) {
    // Explicitly mask to 32-bit for the DR register assignment
    i2c->DR = (uint32_t)(((uint32_t)addr << 1) | (read ? 1U : 0U));

    while (!i2c->SR1_b.ADDR)
        ;

    (void)i2c->SR2;
}

void i2c_write(volatile struct i2c *i2c, uint8_t data) {
    while (!i2c->SR1_b.TXE)
        ;
    i2c->DR = (uint32_t)data;
    while (!i2c->SR1_b.BTF)
        ;
}

uint8_t i2c_read_ack(volatile struct i2c *i2c) {
    i2c->CR1_b.ACK = 1;
    while (!i2c->SR1_b.RXNE)
        ;
    return (uint8_t)(i2c->DR & 0xFFU);
}

uint8_t i2c_read_nack(volatile struct i2c *i2c) {
    i2c->CR1_b.ACK = 0;
    i2c->CR1_b.STOP = 1;
    while (!i2c->SR1_b.RXNE)
        ;
    return (uint8_t)(i2c->DR & 0xFFU);
}

/**
 * @brief Checks if a device is ready/present on the bus.
 * @return true if ACK received, false if NACK or timeout.
 */
bool i2c_is_ready(volatile struct i2c *i2c, uint8_t address) {
    // 1. Clear any existing error flags
    i2c->SR1_b.AF = 0;

    // 2. Generate START
    i2c->CR1_b.START = 1;
    uint32_t timeout = 1000;
    while (!i2c->SR1_b.SB && --timeout)
        ;
    if (timeout == 0)
        return false;

    // 3. Send address (shifted left)
    // We send in Write mode (LSB = 0) just to check for ACK
    i2c->DR = (uint32_t)(address << 1);

    // 4. Wait for ADDR (ACK) or AF (NACK)
    timeout = 1000;
    while (!(i2c->SR1_b.ADDR || i2c->SR1_b.AF) && --timeout)
        ;

    bool found = false;
    if (i2c->SR1_b.ADDR) {
        found = true;
        (void)i2c->SR2; // Clear ADDR flag
    }

    // 5. Generate STOP to release the bus
    i2c->CR1_b.STOP = 1;

    // 6. Reset AF flag for the next scan
    i2c->SR1_b.AF = 0;

    return found;
}

// vim: et ts=4 nowrap autoindent