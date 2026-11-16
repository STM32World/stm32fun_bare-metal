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
void gpio_set_mode(uint16_t pin, uint8_t mode) {
    volatile struct gpio *gp = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin);                            // Pin number
    gp->MODER &= ~(3U << (n * 2));                 // Clear existing setting
    gp->MODER |= (mode & 3) << (n * 2);            // Set new mode
}

void gpio_set_af(uint16_t pin, uint8_t af_num) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    int n = PINNO(pin);

    // Treat AFRL and AFRH as an array of two 32-bit unions
    // AFRL is the first, AFRH is the second.
    volatile uint32_t *afr_reg = (n < 8) ? &gp->AFRL : &gp->AFRH;

    uint32_t shift = (uint32_t)(n & 7) * 4;

    *afr_reg &= ~(15UL << shift);
    *afr_reg |= ((uint32_t)af_num << shift);
}

// Set GPIO output type (push-pull or open-drain)
void gpio_set_otype(uint16_t pin, uint8_t otype) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    uint32_t n = (uint32_t)PINNO(pin);
    gp->OTYPER &= ~BIT(n);                    // Clear existing setting
    gp->OTYPER |= (uint32_t)(otype & 1) << n; // Set new output type
}

// Set GPIO output speed
void gpio_set_speed(uint16_t pin, uint8_t speed) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    uint32_t shift = (uint32_t)PINNO(pin) * 2;
    gp->OSPEEDR &= ~(3UL << shift);                // Clear existing setting
    gp->OSPEEDR |= (uint32_t)(speed & 3) << shift; // Set new speed
}

// Set GPIO pull-up/pull-down
void gpio_set_pull(uint16_t pin, uint8_t pull) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    uint32_t shift = (uint32_t)PINNO(pin) * 2;
    gp->PUPDR &= ~(3UL << shift);               // Clear existing setting
    gp->PUPDR |= (uint32_t)(pull & 3) << shift; // Set new pull
}

// Write value to GPIO pin
void gpio_write(uint16_t pin, bool val) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    gp->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

// Read value from GPIO pin
inline bool gpio_read(uint16_t pin) {
    volatile struct gpio *gp = GPIO(PINBANK(pin));
    return (gp->IDR & (1U << PINNO(pin))) ? true : false;
}

// vim: et ts=4 nowrap autoindent
