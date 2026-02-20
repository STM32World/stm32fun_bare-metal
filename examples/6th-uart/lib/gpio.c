/**
 *
 * GPIO interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "gpio.h"

// Set GPIO pin mode
inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin);                     // Pin number
    gpio->MODER &= ~(3U << (n * 2));        // Clear existing setting
    gpio->MODER |= (mode & 3) << (n * 2);   // Set new mode
}

inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);

    // Treat AFRL and AFRH as an array of two 32-bit unions
    // AFRL is the first, AFRH is the second.
    volatile uint32_t *afr_reg = (n < 8) ? &gpio->AFRL : &gpio->AFRH;

    uint32_t shift = (uint32_t)(n & 7) * 4;

    *afr_reg &= ~(15UL << shift);
    *afr_reg |= ((uint32_t)af_num << shift);
}

// Write value to GPIO pin
inline void gpio_write(uint16_t pin, bool val) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}
