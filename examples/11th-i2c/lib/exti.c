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
 * Route a GPIO pin to its EXTI line, select the trigger edge(s), unmask the line and
 * enable its interrupt in the NVIC. Lines 0-4 each have their own IRQ, lines 5-9 share
 * EXTI9_5 and lines 10-15 share EXTI15_10. Only one port can use a given line number.
 * @param pin: Pin, e.g. PIN('C', 15)
 * @param falling: Trigger on the falling edge
 * @param rising: Trigger on the rising edge
 */
void exti_init(uint16_t pin, bool falling, bool rising) {

    uint32_t pin_no = (uint32_t)PINNO(pin);
    uint32_t bank = (uint32_t)PINBANK(pin);

    // Enable SYSCFG Clock
    RCC->APB2ENR_b.SYSCFGEN = 1; // SYSCFG is on APB2 bus

    // Route the specific GPIO Bank to the EXTI Line
    // Each EXTICR register handles 4 pins (4 bits per pin)
    uint32_t icr_idx = pin_no >> 2;        // e.g., Pin 15 / 4 = index 3
    uint32_t shift = (pin_no & 0x03) << 2; // e.g., (15 % 4) * 4 = shift 12

    // Clear 4 bits and set them to the bank index (A=0, B=1, C=2...)
    SYSCFG->EXTICR[icr_idx] &= ~((uint32_t)0x0FU << shift);
    SYSCFG->EXTICR[icr_idx] |= (bank << shift);

    // Configure the trigger edges
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

    // Enable the NVIC interrupt for this line. Lines 0-4 have their own IRQ,
    // lines 5-9 share EXTI9_5 and lines 10-15 share EXTI15_10.
    uint8_t irq;
    if (pin_no <= 4) {
        irq = (uint8_t)(EXTI0_IRQ + pin_no);
    } else if (pin_no <= 9) {
        irq = EXTI9_5_IRQ;
    } else {
        irq = EXTI15_10_IRQ;
    }
    nvic_enable_irq(irq);
}

/**
 * Check whether the EXTI line of a pin has a pending interrupt.
 * @param pin: Pin, e.g. PIN('C', 15) (only the pin number matters)
 * @return 1 if pending, else 0
 */
int exti_get_pending(uint16_t pin) {
    uint8_t pin_num = (uint8_t)(PINNO(pin));
    return (EXTI->PR & BIT(pin_num)) ? 1 : 0;
}

/**
 * Clear the pending flag of the EXTI line of a pin. Must be done in the interrupt
 * handler, or the interrupt fires again immediately. PR is "write 1 to clear" (rc_w1),
 * so a plain write of the bit is used - no read-modify-write.
 * @param pin: Pin, e.g. PIN('C', 15) (only the pin number matters)
 */
void exti_clear_pending(uint16_t pin) {
    uint8_t pin_num = (uint8_t)(PINNO(pin));
    EXTI->PR = BIT(pin_num);
}

// vim: et ts=4 nowrap autoindent
