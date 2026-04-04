/**
 *
 * EXTI (External Interrupt/Event Controller) implementation for STM32F4xx.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "f4x.h"

/**
 * @brief Initializes an EXTI line for a specific GPIO pin.
 * @param pin Use the PIN(port, pin) macro.
 * @param falling Enable falling edge trigger (1 or 0).
 * @param rising Enable rising edge trigger (1 or 0).
 */
void exti_init(uint16_t pin, uint8_t falling, uint8_t rising) {

    uint32_t pin_no = (uint32_t)PINNO(pin);
    uint32_t bank   = (uint32_t)PINBANK(pin);

    // Enable SYSCFG Clock
    RCC->APB2ENR_b.SYSCFGEN = 1;

    // Route the specific GPIO Bank to the EXTI Line
    // Each EXTICR register handles 4 pins (4 bits per pin)
    uint32_t icr_idx = pin_no >> 2;          // e.g., Pin 15 / 4 = index 3
    uint32_t shift   = (pin_no & 0x03) << 2; // e.g., (15 % 4) * 4 = shift 12
    
    // Clear 4 bits and set them to the bank index (A=0, B=1, C=2...)
    SYSCFG->EXTICR[icr_idx] &= ~((uint32_t)0x0FU << shift);
    SYSCFG->EXTICR[icr_idx] |= (bank << shift);

    // Configure Triggers using your BIT macro
    if (rising) {
        EXTI->RTSR |= BIT(pin_no);
    } else {
        EXTI->RTSR &= ~BIT(pin_no);
    }
    
    if (falling) {
        EXTI->FTSR |= BIT(pin_no);
    } else {
        EXTI->FTSR &= ~BIT(pin_no);
    }

    // Unmask the interrupt line
    EXTI->IMR |= BIT(pin_no);

    // Enable NVIC Interrupt
    // EXTI15_10 is IRQ 40 (Vector 56). ISER[1] handles IRQs 32-63.
    // Logic: 40 - 32 = bit 8
    volatile uint32_t *nvic_iser1 = (volatile uint32_t *)0xE000E104UL;
    *nvic_iser1 |= BIT(40 - 32);
}

/**
 * @brief Checks if a specific EXTI line has a pending interrupt.
 */
int exti_get_pending(uint16_t pin) {
    uint8_t pin_num = (uint8_t)(PINNO(pin));
    return (EXTI->PR & BIT(pin_num)) ? 1 : 0;
}

/**
 * @brief Clears the pending flag for an EXTI line.
 * Note: EXTI->PR is "Write 1 to clear" (rc_w1).
 */
void exti_clear_pending(uint16_t pin) {
    uint8_t pin_num = (uint8_t)(PINNO(pin));
    EXTI->PR = BIT(pin_num);
}

// vim: et sts=4 sw=4 ts=4