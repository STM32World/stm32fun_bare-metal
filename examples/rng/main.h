/**
 * 
 * Systick Blink 16 MHz HSE bare-metal STM32 example
 * 
 * This file contains the project configuration and common includes for the Systick Blink example.
 * 
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 * 
 */

#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <stdbool.h>

/* System clock configuration for STM32F4 running at 168 MHz using HSE and PLL */

enum {
    APB1_PRE = 4, /* AHB clock / 4 */
    APB2_PRE = 2, /* AHB clock / 2 */
};

enum {  // Run at 168 Mhz
    PLL_HSE = 16,
    PLL_M = 16,
    PLL_N = 336,
    PLL_P = 2,
    PLL_Q = 7,
};

/* Miscellaneous calculations */


#define FLASH_LATENCY 5
#define SYS_FREQUENCY ((PLL_HSE * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY (SYS_FREQUENCY / APB2_PRE)
#define APB1_FREQUENCY (SYS_FREQUENCY / APB1_PRE)

// Helper to convert divisor (2, 4, 8, 16) to STM32 PPRE bits
// If divisor is 1, return 0. Else, use the pattern (log2(div) + 3)
#define PPRE_BITS(div) ((div) == 1 ? 0 : (31 - __builtin_clz(div)) + 3)

#include "f4x.h"

#endif /* __MAIN_H */