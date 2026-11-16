/**
 *
 * NVIC driver for STM32F4xx series microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "nvic.h"

/**
 * Enable an interrupt in the NVIC.
 * @param irq_n: IRQ number, e.g. TIM2_IRQ (see vectors.h)
 */
void nvic_enable_irq(uint8_t irq_n) {
    // ISER registers are 32 bits wide.
    // irq_n >> 5 determines which array index (irq_n / 32)
    // irq_n & 0x1F determines the bit position (irq_n % 32)
    NVIC->ISER[irq_n >> 5] = (1UL << (irq_n & 0x1F));
}

/**
 * Disable an interrupt in the NVIC.
 * @param irq_n: IRQ number, e.g. TIM2_IRQ (see vectors.h)
 */
void nvic_disable_irq(uint8_t irq_n) {
    // Writing a 1 to the ICER register disables the IRQ.
    // Writing a 0 has no effect.
    NVIC->ICER[irq_n >> 5] = (1UL << (irq_n & 0x1F));
}

/**
 * Set the priority of an interrupt.
 * @param irq_n: IRQ number, e.g. TIM2_IRQ (see vectors.h)
 * @param priority: 0 (highest) to 15 (lowest)
 */
void nvic_set_priority(uint8_t irq_n, uint8_t priority) {
    // STM32F407 implements 4 bits of priority (the upper 4 bits of the 8-bit field).
    // Priority 0 is the highest, 15 is the lowest.
    if (irq_n < 240) {
        NVIC->IP[irq_n] = (uint8_t)((priority << 4) & 0xFF);
    }
}

// vim: et ts=4 nowrap autoindent
