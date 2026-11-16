/**
 *
 * RNG interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "rng.h"

/**
 * Enable the hardware random number generator. The RNG is clocked from the 48 MHz
 * PLL output (PLL_Q in main.h), so sysclock_init() must have run first.
 */
void rng_init(void) {

    // Enable the RNG clock in the RCC
    RCC->AHB2ENR_b.RNGEN = 1; // Enable RNG clock

    (void)RCC->AHB2ENR;

    // Enable the RNG peripheral
    RNG->CR_b.RNGEN = 1; // Enable RNG by setting the RNGEN bit (bit 0)
}

/**
 * Get a 32-bit random number, waiting until one is available (~40 PLL48 cycles).
 * @return Random number, or 0xDEADBEEF if the RNG reports a clock error
 */
uint32_t rng_get_random(void) {

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

// vim: et ts=4 nowrap autoindent
