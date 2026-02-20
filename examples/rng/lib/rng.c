/**
 *
 * RNG interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "rng.h"

void rng_init() {

    // Enable the RNG clock in the RCC
    RCC->AHB2ENR_b.RNGEN = 1; // Enable RNG clock

    __asm__ volatile("nop"); // Short delay to ensure the clock is stable before accessing the RNG registers

    // Enable the RNG peripheral
    RNG->CR |= BIT(0); // Enable RNG by setting the RNGEN bit (bit 0)
}

uint32_t rng_get_random() {

    RNG->CR_b.RNGEN = 1; // Ensure RNG is enabled

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
