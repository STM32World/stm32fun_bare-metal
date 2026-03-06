/**
 *
 * GPIO interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef __GPIO_H
#define __GPIO_H

#include "f4x.h"

// GPIO peripheral structure
struct gpio {
    union { // MODE register: 2 bits per pin
        volatile uint32_t MODER;
        struct {
            volatile uint32_t PIN0 : 2;
            volatile uint32_t PIN1 : 2;
            volatile uint32_t PIN2 : 2;
            volatile uint32_t PIN3 : 2;
            volatile uint32_t PIN4 : 2;
            volatile uint32_t PIN5 : 2;
            volatile uint32_t PIN6 : 2;
            volatile uint32_t PIN7 : 2;
            volatile uint32_t PIN8 : 2;
            volatile uint32_t PIN9 : 2;
            volatile uint32_t PIN10 : 2;
            volatile uint32_t PIN11 : 2;
            volatile uint32_t PIN12 : 2;
            volatile uint32_t PIN13 : 2;
            volatile uint32_t PIN14 : 2;
            volatile uint32_t PIN15 : 2;
        } MODER_b;
    };
    union { // Output type register: 1 bit per pin
        volatile uint32_t OTYPER;
        struct {
            volatile uint32_t PIN0 : 1;
            volatile uint32_t PIN1 : 1;
            volatile uint32_t PIN2 : 1;
            volatile uint32_t PIN3 : 1;
            volatile uint32_t PIN4 : 1;
            volatile uint32_t PIN5 : 1;
            volatile uint32_t PIN6 : 1;
            volatile uint32_t PIN7 : 1;
            volatile uint32_t PIN8 : 1;
            volatile uint32_t PIN9 : 1;
            volatile uint32_t PIN10 : 1;
            volatile uint32_t PIN11 : 1;
            volatile uint32_t PIN12 : 1;
            volatile uint32_t PIN13 : 1;
            volatile uint32_t PIN14 : 1;
            volatile uint32_t PIN15 : 1;
            volatile uint32_t : 16;
        } OTYPER_b;
    };
    union { // OSPEEDR register: 2 bits per pin
        volatile uint32_t OSPEEDR;
        struct {
            volatile uint32_t PIN0 : 2;
            volatile uint32_t PIN1 : 2;
            volatile uint32_t PIN2 : 2;
            volatile uint32_t PIN3 : 2;
            volatile uint32_t PIN4 : 2;
            volatile uint32_t PIN5 : 2;
            volatile uint32_t PIN6 : 2;
            volatile uint32_t PIN7 : 2;
            volatile uint32_t PIN8 : 2;
            volatile uint32_t PIN9 : 2;
            volatile uint32_t PIN10 : 2;
            volatile uint32_t PIN11 : 2;
            volatile uint32_t PIN12 : 2;
            volatile uint32_t PIN13 : 2;
            volatile uint32_t PIN14 : 2;
            volatile uint32_t PIN15 : 2;
        } OSPEEDR_b;
    };
    union { // PUPDR register: 2 bits per pin
        volatile uint32_t PUPDR;
        struct {
            volatile uint32_t PIN0 : 2;
            volatile uint32_t PIN1 : 2;
            volatile uint32_t PIN2 : 2;
            volatile uint32_t PIN3 : 2;
            volatile uint32_t PIN4 : 2;
            volatile uint32_t PIN5 : 2;
            volatile uint32_t PIN6 : 2;
            volatile uint32_t PIN7 : 2;
            volatile uint32_t PIN8 : 2;
            volatile uint32_t PIN9 : 2;
            volatile uint32_t PIN10 : 2;
            volatile uint32_t PIN11 : 2;
            volatile uint32_t PIN12 : 2;
            volatile uint32_t PIN13 : 2;
            volatile uint32_t PIN14 : 2;
            volatile uint32_t PIN15 : 2;
        } PUPDR_b;
    };
    union { // IDR register: 1 bit per pin for input data, 16 bits reserved
        volatile uint32_t IDR;
        struct {
            volatile uint32_t PIN0 : 1;
            volatile uint32_t PIN1 : 1;
            volatile uint32_t PIN2 : 1;
            volatile uint32_t PIN3 : 1;
            volatile uint32_t PIN4 : 1;
            volatile uint32_t PIN5 : 1;
            volatile uint32_t PIN6 : 1;
            volatile uint32_t PIN7 : 1;
            volatile uint32_t PIN8 : 1;
            volatile uint32_t PIN9 : 1;
            volatile uint32_t PIN10 : 1;
            volatile uint32_t PIN11 : 1;
            volatile uint32_t PIN12 : 1;
            volatile uint32_t PIN13 : 1;
            volatile uint32_t PIN14 : 1;
            volatile uint32_t PIN15 : 1;
            volatile uint32_t : 16;
        } IDR_b;
    };
    union { // ODR register: 1 bit per pin for output data, 16 bits reserved
        volatile uint32_t ODR;
        struct {
            volatile uint32_t PIN0 : 1;
            volatile uint32_t PIN1 : 1;
            volatile uint32_t PIN2 : 1;
            volatile uint32_t PIN3 : 1;
            volatile uint32_t PIN4 : 1;
            volatile uint32_t PIN5 : 1;
            volatile uint32_t PIN6 : 1;
            volatile uint32_t PIN7 : 1;
            volatile uint32_t PIN8 : 1;
            volatile uint32_t PIN9 : 1;
            volatile uint32_t PIN10 : 1;
            volatile uint32_t PIN11 : 1;
            volatile uint32_t PIN12 : 1;
            volatile uint32_t PIN13 : 1;
            volatile uint32_t PIN14 : 1;
            volatile uint32_t PIN15 : 1;
            volatile uint32_t : 16;
        } ODR_b;
    };
    union { // BSRR register: 16 bits for setting pins, 16 bits for resetting pins
        volatile uint32_t BSRR;
        struct {
            volatile uint32_t SET0 : 1;
            volatile uint32_t SET1 : 1;
            volatile uint32_t SET2 : 1;
            volatile uint32_t SET3 : 1;
            volatile uint32_t SET4 : 1;
            volatile uint32_t SET5 : 1;
            volatile uint32_t SET6 : 1;
            volatile uint32_t SET7 : 1;
            volatile uint32_t SET8 : 1;
            volatile uint32_t SET9 : 1;
            volatile uint32_t SET10 : 1;
            volatile uint32_t SET11 : 1;
            volatile uint32_t SET12 : 1;
            volatile uint32_t SET13 : 1;
            volatile uint32_t SET14 : 1;
            volatile uint32_t SET15 : 1;
            volatile uint32_t CLR0 : 1;
            volatile uint32_t CLR1 : 1;
            volatile uint32_t CLR2 : 1;
            volatile uint32_t CLR3 : 1;
            volatile uint32_t CLR4 : 1;
            volatile uint32_t CLR5 : 1;
            volatile uint32_t CLR6 : 1;
            volatile uint32_t CLR7 : 1;
            volatile uint32_t CLR8 : 1;
            volatile uint32_t CLR9 : 1;
            volatile uint32_t CLR10 : 1;
            volatile uint32_t CLR11 : 1;
            volatile uint32_t CLR12 : 1;
            volatile uint32_t CLR13 : 1;
            volatile uint32_t CLR14 : 1;
            volatile uint32_t CLR15 : 1;
        } BSRR_b;
    };
    union { // LCKR register: 16 bits for locking pins, 1 bit for lock key, 15 bits reserved
        volatile uint32_t LCKR;
        struct {
            volatile uint32_t PIN0 : 1;
            volatile uint32_t PIN1 : 1;
            volatile uint32_t PIN2 : 1;
            volatile uint32_t PIN3 : 1;
            volatile uint32_t PIN4 : 1;
            volatile uint32_t PIN5 : 1;
            volatile uint32_t PIN6 : 1;
            volatile uint32_t PIN7 : 1;
            volatile uint32_t PIN8 : 1;
            volatile uint32_t PIN9 : 1;
            volatile uint32_t PIN10 : 1;
            volatile uint32_t PIN11 : 1;
            volatile uint32_t PIN12 : 1;
            volatile uint32_t PIN13 : 1;
            volatile uint32_t PIN14 : 1;
            volatile uint32_t PIN15 : 1;
            volatile uint32_t LCKK : 1;
            volatile uint32_t : 15;
        } LCKR_b;
    };
    union { // AFRL register: 4 bits per pin for alternate function selection
        volatile uint32_t AFRL;
        struct {
            volatile uint32_t AF0 : 4;
            volatile uint32_t AF1 : 4;
            volatile uint32_t AF2 : 4;
            volatile uint32_t AF3 : 4;
            volatile uint32_t AF4 : 4;
            volatile uint32_t AF5 : 4;
            volatile uint32_t AF6 : 4;
            volatile uint32_t AF7 : 4;
        } AFRL_b;
    };
    union { // AFRH register: 4 bits per pin for alternate function selection
        volatile uint32_t AFRH;
        struct {
            volatile uint32_t AF8 : 4;
            volatile uint32_t AF9 : 4;
            volatile uint32_t AF10 : 4;
            volatile uint32_t AF11 : 4;
            volatile uint32_t AF12 : 4;
            volatile uint32_t AF13 : 4;
            volatile uint32_t AF14 : 4;
            volatile uint32_t AF15 : 4;
        } AFRH_b;
    };
};

#define GPIO(bank) ((volatile struct gpio *)(0x40020000 + 0x400 * (bank))) // GPIO base address

// Enum values are per datasheet: 0, 1, 2, 3
enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG,
};

void gpio_write(uint16_t pin, bool val);
void gpio_set_mode(uint16_t pin, uint8_t mode);
void gpio_set_af(uint16_t pin, uint8_t af_num);

#endif /* __GPIO_H */

// vim: et ts=4 nowrap autoindent