/**
 * 
 * STM32F407 Example config and common includes  
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
    AHB_PRE = 1,  /* No division */
    APB1_PRE = 4, /* AHB clock / 4 */
    APB2_PRE = 2, /* AHB clock / 2 */
};

enum {  // Run at 168 Mhz
    PLL_HSE = 16,
    PLL_M = 8,
    PLL_N = 168,
    PLL_P = 2,
};

/* Miscellaneous calculations */

#define FLASH_LATENCY 5

#define SYS_FREQUENCY ((PLL_HSE * PLL_N / PLL_M / PLL_P) * 1000000 / AHB_PRE)
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3)))
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3)))

#include "f4x.h"

#endif /* __MAIN_H */