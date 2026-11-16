/**
 *
 * Generic timer driver for the STM32F407 (TIM1-TIM14): periodic update events and interrupts.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "timer.h"

/**
 * Everything the driver needs to know about one timer instance.
 */
struct timer_info {
    volatile struct timer *tim; // The instance
    bool apb2;                  // Clocked from APB2 (else APB1)
    uint8_t en_bit;             // Clock enable bit in RCC APB1ENR / APB2ENR
    bool bits32;                // 32-bit counter (TIM2 and TIM5), else 16-bit
    uint8_t irq;                // IRQ that carries the update interrupt
};

// All timers of the STM32F407. Several timers share an IRQ (and so a handler name in
// vectors.h), e.g. the update interrupt of TIM1 arrives in tim1_up_tim10_irq_handler().
static const struct timer_info timer_table[] = {
    { TIM1, true, 0, false, TIM1_UP_TIM10_IRQ },
    { TIM2, false, 0, true, TIM2_IRQ },
    { TIM3, false, 1, false, TIM3_IRQ },
    { TIM4, false, 2, false, TIM4_IRQ },
    { TIM5, false, 3, true, TIM5_IRQ },
    { TIM6, false, 4, false, TIM6_DAC_IRQ },
    { TIM7, false, 5, false, TIM7_IRQ },
    { TIM8, true, 1, false, TIM8_UP_TIM13_IRQ },
    { TIM9, true, 16, false, TIM1_BRK_TIM9_IRQ },
    { TIM10, true, 17, false, TIM1_UP_TIM10_IRQ },
    { TIM11, true, 18, false, TIM1_TRG_COM_TIM11_IRQ },
    { TIM12, false, 6, false, TIM8_BRK_TIM12_IRQ },
    { TIM13, false, 7, false, TIM8_UP_TIM13_IRQ },
    { TIM14, false, 8, false, TIM8_TRG_COM_TIM14_IRQ },
};

/**
 * Look up a timer instance in the table.
 * @return The table entry, or NULL for an unknown instance
 */
static const struct timer_info *timer_get_info(volatile struct timer *tim) {
    for (size_t i = 0; i < sizeof(timer_table) / sizeof(timer_table[0]); ++i)
        if (timer_table[i].tim == tim)
            return &timer_table[i];
    return NULL;
}

/**
 * Initialize a timer to overflow (generate an update event) at the given frequency.
 *
 * When possible the counter runs at 1 MHz, so CNT counts microseconds - this works for
 * 16 Hz to 500 kHz on 16-bit timers (down to 1 Hz on the 32-bit TIM2/TIM5).
 * Outside that range the prescaler is chosen as small as possible for the best
 * resolution. The frequency is exact when the timer clock divides evenly by it,
 * otherwise it is rounded down to the nearest achievable value.
 *
 * Also enables the timer's clock and forces an update so the settings take effect
 * immediately. The timer is not started - call timer_enable().
 *
 * @param tim: Timer instance, e.g. TIM2
 * @param hz: Update frequency in Hz, from 1 up to half the timer clock
 * @return TIM_OK, or TIM_ERR_PARAM for an unknown timer or an impossible frequency
 */
int timer_init(volatile struct timer *tim, uint32_t hz) {
    const struct timer_info *info = timer_get_info(tim);
    if (info == NULL)
        return TIM_ERR_PARAM;

    // Timer clock: APB clock, doubled when the APB prescaler is not 1 (see main.h)
    uint32_t clk = info->apb2 ? TIM_APB2_FREQ : TIM_APB1_FREQ;
    uint64_t max_arr = info->bits32 ? 0xFFFFFFFFu : 0xFFFFu;

    if (hz == 0 || hz > clk / 2) // ARR must be at least 1
        return TIM_ERR_PARAM;

    uint32_t psc, arr;
    if (hz <= 500000 && clk % 1000000 == 0 && 1000000 / hz - 1 <= max_arr) {
        // Preferred: 1 MHz counter clock (up to 500 kHz, as ARR = 0 stops the counter)
        psc = clk / 1000000 - 1;
        arr = 1000000 / hz - 1;
    } else {
        // Smallest prescaler that makes the period fit in ARR
        uint64_t ticks = clk / hz; // Timer clocks per period
        uint64_t div = (ticks + max_arr) / (max_arr + 1);
        if (div > 0x10000) // PSC is 16 bits on every timer
            return TIM_ERR_PARAM;
        psc = (uint32_t)(div - 1);
        arr = (uint32_t)(ticks / div - 1);
    }

    // Enable the clock gate before touching the registers, then read back as a delay
    volatile uint32_t *enr = info->apb2 ? &RCC->APB2ENR : &RCC->APB1ENR;
    *enr |= BIT(info->en_bit);
    (void)*enr;

    tim->PSC = psc;
    tim->ARR = arr;

    // Force an update to load PSC (it is buffered), then clear the flag that sets
    tim->EGR = TIM_EGR_UG;
    timer_clear_update(tim);

    return TIM_OK;
}

/**
 * Initialize a timer and enable its update interrupt, including the NVIC. The IRQ is
 * looked up from the instance - see timer_table for which handler it arrives in.
 * The timer is not started - call timer_enable().
 * @param tim: Timer instance, e.g. TIM2
 * @param hz: Interrupt frequency in Hz
 * @return TIM_OK or TIM_ERR_PARAM (see timer_init())
 */
int timer_setup_interrupt(volatile struct timer *tim, uint32_t hz) {
    int rc = timer_init(tim, hz);
    if (rc != TIM_OK)
        return rc;

    tim->DIER_b.UIE = 1;                       // Update interrupt enable in the timer
    nvic_enable_irq(timer_get_info(tim)->irq); // ...and in the NVIC

    return TIM_OK;
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
