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

#include <stdbool.h>
#include <stdint.h>

/* System clock configuration for STM32F4 running at 168 MHz using HSE and PLL */

enum {
    APB1_PRE = 4, /* AHB clock / 4 */
    APB2_PRE = 2, /* AHB clock / 2 */
};

enum {            // Run at 168 Mhz
    PLL_HSE = 16, // HSE frequency in MHz (16 MHz crystal)
    PLL_M = 16,   // PLL input divider (16 MHz / 16 = 1 MHz)
    PLL_N = 336,  // PLL multiplier (1 MHz * 336 = 336 MHz)
    PLL_P = 2,    // PLL main system clock divider (336 MHz / 2 = 168 MHz for SYSCLK)
    PLL_Q = 7,    // PLL48CLK divider (336 MHz / 7 = 48 MHz for USB, RNG, etc.)
};

/* Miscellaneous calculations */

#define FLASH_LATENCY 5
#define SYS_FREQUENCY ((PLL_HSE * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY (SYS_FREQUENCY / APB2_PRE)
#define APB1_FREQUENCY (SYS_FREQUENCY / APB1_PRE)
#define CLK48 (PLL_HSE * PLL_N / PLL_M / PLL_Q * 1000000) // 48 MHz clock for USB, RNG, etc.

// Helper to convert divisor (2, 4, 8, 16) to STM32 PPRE bits
// If divisor is 1, return 0. Else, use the pattern (log2(div) + 3)
#define PPRE_BITS(div) ((div) == 1 ? 0 : (31 - __builtin_clz(div)) + 3)

#include "f4x.h"

#endif /* __MAIN_H */