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

    // Enable the clock for the specified timer and determine the input clock frequency
    switch ((uintptr_t)tim) {
    case (uintptr_t)TIM1:
        RCC->APB2ENR_b.TIM1EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIM2:
        RCC->APB1ENR_b.TIM2EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIM3:
        RCC->APB1ENR_b.TIM3EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIM4:
        RCC->APB1ENR_b.TIM4EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIM5:
        RCC->APB1ENR_b.TIM5EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIM6:
        RCC->APB1ENR_b.TIM6EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIM7:
        RCC->APB1ENR_b.TIM7EN = 1;
        freq = TIM_APB1_FREQ;
        break;
    case (uintptr_t)TIM8:
        RCC->APB2ENR_b.TIM8EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIM9:
        RCC->APB2ENR_b.TIM9EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIM10:
        RCC->APB2ENR_b.TIM10EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    case (uintptr_t)TIM11:
        RCC->APB2ENR_b.TIM11EN = 1;
        freq = TIM_APB2_FREQ;
        break;
    default:
        freq = 0; // Invalid timer
    }

    // Return the input clock frequency for the timer (0 if invalid timer)
    return freq;
}

/**
 * Initialize the timer with the specified frequency (in Hz).
 * This sets up the prescaler and auto-reload register to achieve the desired frequency.
 * It also forces an update event to load the new settings.
 * @param tim: Pointer to the timer struct (e.g. TIM2)
 * @param hz: Desired frequency in Hz (e.g. 1000 for 1 ms period)
 */
void timer_init(volatile struct timer *tim, uint32_t hz) {

    // Enable the clock gate first!
    uint32_t input_clk = _timer_enable_and_get_clock(tim);

    // Configure timing (1MHz base)
    tim->PSC = (input_clk / 1000000) - 1;
    tim->ARR = (1000000 / hz) - 1;

    // Force update and clear flag
    tim->EGR = TIM_EGR_UG;
    timer_clear_update(tim);
}

/**
 * Configure Periodic Interrupt
 * @param irq_num: The IRQ number (e.g. TIM2_IRQ)
 */
void timer_setup_interrupt(volatile struct timer *tim, uint32_t hz, uint8_t irq_num) {
    // Initialize timing
    timer_init(tim, hz);

    // Enable "Update Interrupt" in the timer hardware
    tim->DIER_b.UIE = 1;

    // Enable the IRQ in the NVIC
    nvic_enable_irq(irq_num);
}

/**
 * Clear the update interrupt flag.
 * SR is "write 0 to clear" (rc_w0): writing 1 to the other bits leaves them alone,
 * so a plain write avoids the read-modify-write race of a bitfield access.
 * @param tim: Pointer to the timer struct (e.g. TIM2)
 */
void timer_clear_update(volatile struct timer *tim) {
    tim->SR = ~TIM_SR_UIF;
}

/**
 * Enable the timer (start counting)
 * @param tim: Pointer to the timer struct (e.g. TIM2)
 */
void timer_enable(volatile struct timer *tim) {
    tim->CR1_b.CEN = 1;
}

/**
 * Disable the timer (stop counting)
 * @param tim: Pointer to the timer struct (e.g. TIM2)
 */
void timer_disable(volatile struct timer *tim) {
    tim->CR1_b.CEN = 0;
}

// vim: et ts=4 nowrap autoindent
