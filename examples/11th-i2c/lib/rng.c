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
    RNG->CR_b.RNGEN = 1; // Enable RNG (RNGEN, bit 2)
}

/**
 * Get a 32-bit random number, waiting until one is available (~40 PLL48 cycles).
 *
 * Two errors are handled:
 *  - Clock error (CECS): the RNG clock is too slow - the PLL is not set up for 48 MHz.
 *    That will not fix itself, so give up.
 *  - Seed error (SEIS): the analog noise source produced a bad sequence. The reference
 *    manual's recovery is to clear the flag and restart the RNG by clearing and setting
 *    RNGEN; the data that was being generated is discarded.
 * @return Random number, or 0xDEADBEEF if the RNG reports a clock error
 */
uint32_t rng_get_random(void) {
    uint32_t sr;

    // Wait until data is ready
    while (!((sr = RNG->SR) & RNG_SR_DRDY)) {
        if (sr & RNG_SR_CECS)
            return 0xDEADBEEF; // PLL not configured correctly for 48 MHz

        if (sr & RNG_SR_SEIS) {
            RNG->SR = ~RNG_SR_SEIS; // Clear the seed error flag (rc_w0)
            RNG->CR_b.RNGEN = 0;    // Restart the generator
            RNG->CR_b.RNGEN = 1;
        }
    }

    // Read and return the random number
    return RNG->DR;
}

// vim: et ts=4 nowrap autoindent
