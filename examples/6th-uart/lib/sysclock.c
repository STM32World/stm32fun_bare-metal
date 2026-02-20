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

    // Enable FPU and Flash Latency (Keep original guide code here)
    SCB->CPACR.reg |= ((3UL << 10 * 2) | (3UL << 11 * 2));

    FLASH->ACR.reg |= FLASH_LATENCY | BIT(8) | BIT(9); // Flash latency, prefetch

    // Enable HSE
    RCC->CR.reg |= BIT(16); // Set HSEON
    while (!(RCC->CR.reg & BIT(17)))
        (void)0; // Wait for HSERDY

    // AHB = /1, APB2 = /2 (84MHz), APB1 = /4 (42MHz)
    RCC->CFGR.reg |= (0 << 4) |                    // HPRE (AHB)
                 (PPRE_BITS(APB2_PRE) << 13) | // PPRE2 (APB2)
                 (PPRE_BITS(APB1_PRE) << 10);  // PPRE1 (APB1)

    // Configure PLL
    // Clear and set M, N, P, and importantly: Set Bit 22 to select HSE as source
    RCC->PLLCFGR.reg = (PLL_M << 0) | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | BIT(22);

    // Enable PLL
    RCC->CR.reg |= BIT(24); // Set PLLON
    while (!(RCC->CR.reg & BIT(25)))
        ; // Wait for PLLRDY

    // Select PLL as System Clock
    RCC->CFGR.reg &= ~(uint32_t)3; // Clear SW bits
    RCC->CFGR.reg |= 2;            // Select PLL (0b10)
    while ((RCC->CFGR.reg & (3 << 2)) != (2 << 2))
        (void)0; // Wait for SWS to indicate PLL

    RCC->AHB1ENR.reg |= BIT(0) | BIT(1) | BIT(2); // Enable clocks for GPIOA, GPIOB, GPIOC
}

// vim: ts=4 sw=4 noexpandtab