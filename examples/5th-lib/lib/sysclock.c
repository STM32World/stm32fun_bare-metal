
#include "sysclock.h"

void sysclock_init(void) {

    // Enable FPU and Flash Latency (Keep original guide code here)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);  // Flash latency, prefetch

    // Enable HSE
    RCC->CR |= BIT(16);            // Set HSEON
    while (!(RCC->CR & BIT(17)));  // Wait for HSERDY

    // Configure PLL
    // Clear and set M, N, P, and importantly: Set Bit 22 to select HSE as source
    RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | BIT(22);

    // Enable PLL
    RCC->CR |= BIT(24);            // Set PLLON
    while (!(RCC->CR & BIT(25)));  // Wait for PLLRDY

    // Select PLL as System Clock
    RCC->CFGR &= ~(uint32_t)3;                   // Clear SW bits
    RCC->CFGR |= 2;                              // Select PLL (0b10)
    while ((RCC->CFGR & (3 << 2)) != (2 << 2));  // Wait for SWS to indicate PLL
    
}
