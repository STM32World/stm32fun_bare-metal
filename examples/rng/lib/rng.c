/**
 *
 * RNG interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "rng.h"

#include <stdio.h>

void rng_init() {

    // Enable the RNG clock in the RCC
    RCC->AHB2ENR_b.RNGEN = 1; // Enable RNG clock

    //(void)RCC->AHB2ENR;
    //__asm__ volatile("nop; nop; nop; nop");

    // Enable the RNG peripheral
    RNG->CR_b.RNGEN = 1; // Enable RNG by setting the RNGEN bit (bit 0)
}

uint32_t rng_get_random() {

    // Wait until data is ready
    while (!RNG->SR_b.DRDY) {
        if (RNG->SR_b.CECS) {
            // Your PLL is not configured correctly for 48MHz!
            return 0xDEADBEEF;
        }
    }

    // Read and return the random number
    return RNG->DR;
}

// vim: et ts=3 nowrap
