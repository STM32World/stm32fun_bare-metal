#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <stdbool.h>

#include "f4x.h"

/* System clock configuration for STM32F4 running at 168 MHz using HSE and PLL */

enum {
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
#define SYS_FREQUENCY ((PLL_HSE * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3)))
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3)))

#endif /* __MAIN_H */