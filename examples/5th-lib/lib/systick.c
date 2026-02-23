/**
 *
 * SysTick (System Timer) interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "systick.h"

// Global tick counter (in milliseconds)
volatile uint32_t s_ticks = 0;

// Initialize SysTick timer to generate interrupts every 'ticks' clock cycles
void systick_init(uint32_t ticks) {
    SYSTICK->RVR = ticks - 1;                 // Set reload register
    SYSTICK->CVR = 0;                         // Clear current value register
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2);  // Enable SysTick, use processor clock
    // We dont need to enable anything in RCC since its a core peripheral and is clocked by the core
    // RCC->APB2ENR |= BIT(14);                  // Enable SYSTICK clock
}

// Systick interrupt handler
_weak void systick_handler(void) { // Weak attribute allows user to override this handler
    ++s_ticks;  // Will increase every 1 ms
}
