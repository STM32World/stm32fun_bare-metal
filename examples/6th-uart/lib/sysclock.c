/**
 *
 * SysClock interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "sysclock.h"

void sysclock_init(void) {

    // Enable FPU
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));

    // 2. Flash Latency & Caches for 168MHz
    FLASH->ACR_b.LATENCY = 5;
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
    RCC->PLLCFGR_b.PLLSRC = 1; // Use HSE as PLL source

    // Enable PLL
    RCC->CR_b.PLLON = 1;
    while (!RCC->CR_b.PLLRDY)
        (void)0;

    // Select PLL as System Clock
    RCC->CFGR_b.SW = 2;
    while (RCC->CFGR_b.SWS != 2)
        (void)0;

    // Enable Peripheral Clocks
    RCC->AHB1ENR_b.GPIOAEN = 1;
    RCC->AHB1ENR_b.GPIOBEN = 1;
    RCC->AHB1ENR_b.GPIOCEN = 1;
}

// vim: ts=4 sw=4 noexpandtab