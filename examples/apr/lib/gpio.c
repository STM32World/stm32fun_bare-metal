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
    volatile struct gpio *gp = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin);                     // Pin number
    gp->MODER &= ~(3U << (n * 2));        // Clear existing setting
    gp->MODER |= (mode & 3) << (n * 2);   // Set new mode
}

inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    int n = PINNO(pin);

    // Treat AFRL and AFRH as an array of two 32-bit unions
    // AFRL is the first, AFRH is the second.
    volatile uint32_t *afr_reg = (n < 8) ? &gp->AFRL : &gp->AFRH;

    uint32_t shift = (uint32_t)(n & 7) * 4;

    *afr_reg &= ~(15UL << shift);
    *afr_reg |= ((uint32_t)af_num << shift);
}

// Write value to GPIO pin
inline void gpio_write(uint16_t pin, bool val) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    gp->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

// vim: et ts=4 nowrap autoindent