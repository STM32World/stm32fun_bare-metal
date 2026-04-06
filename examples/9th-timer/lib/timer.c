/**
 *
 * Generic Timer implementation for STM32F407.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "timer.h"

/**
 * Internal helper to enable the clock and return the frequency.
 */
static uint32_t _timer_enable_and_get_clock(volatile struct timer *tim) {

    uint32_t freq = 0; // Return frequency in Hz - 0 if invalid timer

    switch ((uintptr_t)tim) {
    case (uintptr_t)TIMER1:
        RCC->APB2ENR_b.TIM1EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIMER2:
        RCC->APB1ENR_b.TIM2EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIMER3:
        RCC->APB1ENR_b.TIM3EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIMER4:
        RCC->APB1ENR_b.TIM4EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIMER5:
        RCC->APB1ENR_b.TIM5EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIMER6:
        RCC->APB1ENR_b.TIM6EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIMER7:
        RCC->APB1ENR_b.TIM7EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIMER8:
        RCC->APB2ENR_b.TIM8EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIMER9:
        RCC->APB2ENR_b.TIM9EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIMER10:
        RCC->APB2ENR_b.TIM10EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIMER11:
        RCC->APB2ENR_b.TIM11EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    default:
        freq = 0; // Invalid timer
    }

    return freq;
}

/**
 * Initialize the timer with the specified frequency (in Hz).
 * This sets up the prescaler and auto-reload register to achieve the desired frequency.
 * It also forces an update event to load the new settings.
 * @param tim: Pointer to the timer struct (e.g. TIMER2)
 * @param hz: Desired frequency in Hz (e.g. 1000 for 1 ms period)
 */
void timer_init(volatile struct timer *tim, uint32_t hz) {

    // Enable the clock gate first!
    uint32_t input_clk = _timer_enable_and_get_clock(tim);

    // Configure timing (1MHz base)
    tim->PSC = (input_clk / 1000000) - 1;
    tim->ARR = (1000000 / hz) - 1;

    // Force update and clear flag
    tim->EGR_b.UG = 1;
    tim->SR_b.UIF = 0;
}

/**
 * Configure Periodic Interrupt
 * @param irq_num: The IRQ number (e.g. 28 for TIM2)
 */
void timer_setup_interrupt(volatile struct timer *tim, uint32_t hz, uint8_t irq_num) {
    // Initialize timing
    timer_init(tim, hz);

    // Enable "Update Interrupt" in the timer hardware
    tim->DIER_b.UIE = 1;

    // Enable the IRQ in the NVIC ISER (Interrupt Set-Enable Register)
    // ISER address is 0xE000E100. There are 8 registers (ISER0-7).
    volatile uint32_t *nvic_iser = (volatile uint32_t *)0xE000E100;
    nvic_iser[irq_num >> 5] |= (1 << (irq_num & 0x1F));
}

void timer_enable(volatile struct timer *tim) {
    tim->CR1_b.CEN = 1;
}

void timer_disable(volatile struct timer *tim) {
    tim->CR1_b.CEN = 0;
}

// vim: ts=4 et nowrap