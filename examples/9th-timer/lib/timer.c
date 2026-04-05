/**
 *
 * Generic Timer implementation for STM32F407.
 * * Includes address-based clock detection and interrupt setup.
 *
 */

#include "timer.h"

/**
 * Internal helper to enable the clock and return the frequency.
 * On STM32F407:
 * APB1 base is 0x40000000. RCC_APB1ENR is at RCC_BASE + 0x40.
 * APB2 base is 0x40010000. RCC_APB2ENR is at RCC_BASE + 0x44.
 */
static uint32_t _timer_enable_and_get_clock(volatile struct timer *tim) {
    uintptr_t addr = (uintptr_t)tim;

    // Using raw addresses for RCC to keep it standalone
    volatile uint32_t *rcc_apb1enr = (uint32_t *)(0x40023800 + 0x40);
    volatile uint32_t *rcc_apb2enr = (uint32_t *)(0x40023800 + 0x44);

    // APB2 Timers
    if (addr >= 0x40010000 && addr <= 0x40015000) {
        // Map address to RCC Bit
        if (addr == 0x40010000)
            *rcc_apb2enr |= (1 << 0); // TIM1
        if (addr == 0x40010400)
            *rcc_apb2enr |= (1 << 1); // TIM8
        if (addr == 0x40014000)
            *rcc_apb2enr |= (1 << 16); // TIM9
        if (addr == 0x40014400)
            *rcc_apb2enr |= (1 << 17); // TIM10
        if (addr == 0x40014800)
            *rcc_apb2enr |= (1 << 18); // TIM11

        return TIM_APB2_FREQ;
    }

    // APB1 Timers
    if (addr >= 0x40000000 && addr <= 0x40006000) {
        // Bit 0=TIM2, 1=TIM3, 2=TIM4, 3=TIM5, 4=TIM6, 5=TIM7...
        uint32_t bit = (addr - 0x40000000) / 0x400;
        *rcc_apb1enr |= (1 << bit);

        return TIM_APB1_FREQ;
    }

    return 0;
}

/**
 * Internal helper to determine the input clock frequency for a given timer
 * using the provided #define macros.
 */
static uint32_t _timer_get_clock(volatile struct timer *tim) {

    uintptr_t addr = (uintptr_t)tim;

    // APB2 Timers: TIM1, TIM8, TIM9, TIM10, TIM11
    // Range: 0x4001 0000 - 0x4001 4C00
    if (addr >= 0x40010000 && addr <= 0x40015000) {
        return TIM_APB2_FREQ;
    }

    // APB1 Timers: TIM2-7, TIM12-14
    // Range: 0x4000 0000 - 0x4000 5C00
    return TIM_APB1_FREQ;
}

void timer_init(volatile struct timer *tim, uint32_t hz) {
    // 1. Enable the clock gate first!
    uint32_t input_clk = _timer_enable_and_get_clock(tim);

    // 2. Configure timing (1MHz base)
    tim->PSC = (input_clk / 1000000) - 1;
    tim->ARR = (1000000 / hz) - 1;

    // 3. Force update and clear flag
    tim->EGR_b.UG = 1;
    tim->SR_b.UIF = 0;
}

/**
 * Configure Periodic Interrupt
 * @param irq_num: The IRQ number (e.g. 28 for TIM2)
 */
void timer_setup_interrupt(volatile struct timer *tim, uint32_t hz, uint8_t irq_num) {
    // 1. Initialize timing
    timer_init(tim, hz);

    // 2. Enable "Update Interrupt" in the timer hardware
    tim->DIER_b.UIE = 1;

    // 3. Enable the IRQ in the NVIC ISER (Interrupt Set-Enable Register)
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

/**
 * Example ISR Implementation Logic:
 * Note: This name must match your startup_stm32f407xx.s vector table.
 * * void TIM2_IRQHandler(void) {
 * if (TIMER2->SR_b.UIF) {
 * TIMER2->SR_b.UIF = 0; // Clear flag
 * // Do work...
 * }
 * }
 */