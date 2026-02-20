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

    SYSTICK->CSR.field.ENABLE = 0;    // Disable SysTick during setup
    SYSTICK->RVR.reg = ticks - 1;     // Set reload register
    SYSTICK->CVR.reg = 0;             // Clear current value register
    SYSTICK->CSR.field.CLKSOURCE = 1; // Use processor clock
    SYSTICK->CSR.field.TICKINT = 1;   // Enable SysTick interrupt
    SYSTICK->CSR.field.ENABLE = 1;    // Enable SysTick timer

    //  We dont need to enable anything in RCC since its a core peripheral and is clocked by the core
}

// Systick interrupt handler
void systick_handler(void) {
    ++s_ticks; // Will increase every 1 ms
}
