/**
 *
 * SysClock interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "sysclock.h" // PLL settings come from main.h (through f4x.h)

/**
 * Initialize the system clock according to the configuration in main.h: HSE crystal
 * through the main PLL, flash wait states and caches for the core clock, and the AHB/APB
 * prescalers. (The FPU is enabled in _reset and GPIO port clocks by the gpio functions.)
 */
void sysclock_init(void) {

    // Flash wait states must be set BEFORE the clock goes up, plus prefetch and caches
    FLASH->ACR_b.LATENCY = FLASH_LATENCY;
    FLASH->ACR_b.PRFTEN = 1;
    FLASH->ACR_b.ICEN = 1;
    FLASH->ACR_b.DCEN = 1;

    // Enable HSE
    RCC->CR_b.HSEON = 1;
    while (!RCC->CR_b.HSERDY)
        (void)0;

    // Bus Prescalers: AHB /1, APB2 /2, APB1 /4
    RCC->CFGR_b.HPRE = 0;
    RCC->CFGR_b.PPRE2 = PPRE_BITS(APB2_PRE); // /2
    RCC->CFGR_b.PPRE1 = PPRE_BITS(APB1_PRE); // /4

    // Configure Main PLL
    RCC->PLLCFGR_b.PLLM = PLL_M;
    RCC->PLLCFGR_b.PLLN = PLL_N;
    RCC->PLLCFGR_b.PLLP = (PLL_P >> 1) - 1;
    RCC->PLLCFGR_b.PLLSRC = 1;   // Use HSE as PLL source
    RCC->PLLCFGR_b.PLLQ = PLL_Q; // For 48 MHz peripherals

    // Enable PLL
    RCC->CR_b.PLLON = 1;
    while (!RCC->CR_b.PLLRDY)
        (void)0;

    // Select PLL as System Clock
    RCC->CFGR_b.SW = 2;
    while (RCC->CFGR_b.SWS != 2)
        (void)0;
}

// vim: et ts=4 nowrap autoindent
