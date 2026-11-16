/**
 *
 * GPIO interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "gpio.h"

/**
 * Get the GPIO port of a pin, enabling the port's clock first if needed.
 *
 * Every function in this file goes through here, so a pin on any port just works:
 * writes to a port whose clock is off are silently ignored by the hardware, which is
 * an easy bug to make (sysclock_init() used to enable only GPIOA-C, so e.g. UART5 on
 * PD2 did nothing). The read back after enabling is the delay the reference manual
 * asks for before the peripheral registers can be accessed.
 * @param pin: Pin, e.g. PIN('C', 13)
 * @return Pointer to the port's registers
 */
static volatile struct gpio *gpio_port(uint16_t pin) {
    uint32_t bank = (uint32_t)PINBANK(pin);

    if (!(RCC->AHB1ENR & BIT(bank))) { // GPIOAEN..GPIOIEN are bits 0-8
        RCC->AHB1ENR |= BIT(bank);
        (void)RCC->AHB1ENR;
    }
    return GPIO(bank);
}

/**
 * Set the mode of a GPIO pin.
 * @param pin: Pin, e.g. PIN('C', 13)
 * @param mode: GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF or GPIO_MODE_ANALOG
 */
void gpio_set_mode(uint16_t pin, uint8_t mode) {
    volatile struct gpio *gp = gpio_port(pin);     // GPIO port (clock enabled)
    int n = PINNO(pin);                            // Pin number
    gp->MODER &= ~(3U << (n * 2));                 // Clear existing setting
    gp->MODER |= (mode & 3) << (n * 2);            // Set new mode
}

/**
 * Select the alternate function of a pin (used when the mode is GPIO_MODE_AF).
 * Pins 0-7 are configured in AFRL, pins 8-15 in AFRH, 4 bits per pin.
 * @param pin: Pin, e.g. PIN('B', 6)
 * @param af_num: Alternate function number 0-15 (see the datasheet's AF table)
 */
void gpio_set_af(uint16_t pin, uint8_t af_num) {
    volatile struct gpio *gp = gpio_port(pin);
    int n = PINNO(pin);

    // Treat AFRL and AFRH as an array of two 32-bit unions
    // AFRL is the first, AFRH is the second.
    volatile uint32_t *afr_reg = (n < 8) ? &gp->AFRL : &gp->AFRH;

    uint32_t shift = (uint32_t)(n & 7) * 4;

    *afr_reg &= ~(15UL << shift);
    *afr_reg |= ((uint32_t)(af_num & 15) << shift);
}

/**
 * Set the output type of a pin.
 * @param pin: Pin, e.g. PIN('B', 6)
 * @param otype: GPIO_OTYPE_PUSH_PULL or GPIO_OTYPE_OPEN_DRAIN (needed for I2C)
 */
void gpio_set_otype(uint16_t pin, uint8_t otype) {
    volatile struct gpio *gp = gpio_port(pin);
    uint32_t n = (uint32_t)PINNO(pin);
    gp->OTYPER &= ~BIT(n);                    // Clear existing setting
    gp->OTYPER |= (uint32_t)(otype & 1) << n; // Set new output type
}

/**
 * Set the output speed (slew rate) of a pin. Faster edges are needed for fast
 * peripherals, slower ones reduce noise.
 * @param pin: Pin, e.g. PIN('B', 6)
 * @param speed: GPIO_SPEED_LOW, _MEDIUM, _HIGH or _VERY_HIGH
 */
void gpio_set_speed(uint16_t pin, uint8_t speed) {
    volatile struct gpio *gp = gpio_port(pin);
    uint32_t shift = (uint32_t)PINNO(pin) * 2;
    gp->OSPEEDR &= ~(3UL << shift);                // Clear existing setting
    gp->OSPEEDR |= (uint32_t)(speed & 3) << shift; // Set new speed
}

/**
 * Enable the internal pull-up or pull-down resistor of a pin (~40 kOhm).
 * @param pin: Pin, e.g. PIN('B', 6)
 * @param pull: GPIO_PULL_NONE, GPIO_PULL_UP or GPIO_PULL_DOWN
 */
void gpio_set_pull(uint16_t pin, uint8_t pull) {
    volatile struct gpio *gp = gpio_port(pin);
    uint32_t shift = (uint32_t)PINNO(pin) * 2;
    gp->PUPDR &= ~(3UL << shift);               // Clear existing setting
    gp->PUPDR |= (uint32_t)(pull & 3) << shift; // Set new pull
}

/**
 * Set an output pin high or low. Uses BSRR, so it is atomic - no read-modify-write,
 * safe to use from interrupts.
 * @param pin: Pin, e.g. PIN('C', 13)
 * @param val: true = high, false = low
 */
void gpio_write(uint16_t pin, bool val) {
    volatile struct gpio *gp = gpio_port(pin);
    gp->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

/**
 * Read the level of a pin. Works in every mode: in output mode it returns the actual
 * pin level (which for open-drain may differ from what was written).
 * @param pin: Pin, e.g. PIN('C', 15)
 * @return true if the pin is high
 */
bool gpio_read(uint16_t pin) {
    volatile struct gpio *gp = gpio_port(pin);
    return (gp->IDR & (1U << PINNO(pin))) ? true : false;
}

// vim: et ts=4 nowrap autoindent
