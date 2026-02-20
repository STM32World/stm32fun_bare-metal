/**
 *
 * RNG interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef RNG_H
#define RNG_H

#include "f4x.h"

struct rng {
    union {
        volatile uint32_t CR; // Control register
        struct {
            volatile uint32_t RNGEN : 1; // Random number generator enable
            volatile uint32_t IE : 1;    // Interrupt enable
            volatile uint32_t : 30;      // Reserved
        } CR_b;
    };
    union {
        volatile uint32_t SR; // Status register
        struct {
            volatile uint32_t DRDY : 1; // Data ready
            volatile uint32_t CECS : 1; // Clock error current status
            volatile uint32_t SECS : 1; // Seed error current status
            volatile uint32_t CEIS : 1; // Clock error interrupt status
            volatile uint32_t SEIS : 1; // Seed error interrupt status
            volatile uint32_t : 27;     // Reserved
        } SR_b;
    };
    union {
        volatile uint32_t DR; // Data register (read-only)
        struct {
            volatile uint32_t RNDATA : 32; // Random data output
        } DR_b;
    };
};

#define RNG ((volatile struct rng *)0x50060800) // RNG base address

void rng_init();
uint32_t rng_get_random();

#endif // RNG_H