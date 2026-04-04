/**
 *
 * NVIC interface for STM32F407 (Cortex-M4).
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef NVIC_H
#define NVIC_H

#include "f4x.h"

struct nvic {
    volatile uint32_t ISER[8];       // Interrupt Set-Enable Registers
    uint32_t _reserved0[24];
    volatile uint32_t ICER[8];       // Interrupt Clear-Enable Registers
    uint32_t _reserved1[24];
    volatile uint32_t ISPR[8];       // Interrupt Set-Pending Registers
    uint32_t _reserved2[24];
    volatile uint32_t ICPR[8];       // Interrupt Clear-Pending Registers
    uint32_t _reserved3[24];
    volatile uint32_t IABR[8];       // Interrupt Active Bit Registers
    uint32_t _reserved4[56];
    volatile uint8_t  IP[240];       // Interrupt Priority Registers (8-bit wide)
    uint32_t _reserved5[644];
    volatile uint32_t STIR;          // Software Trigger Interrupt Register
};

// The NVIC base address is defined by the ARM Cortex-M4 Architecture
#define NVIC ((volatile struct nvic *)0xE000E100)

/**
 * @brief Enables a specific IRQ
 * @param irq_n The IRQ number (0-239)
 */
void nvic_enable_irq(uint8_t irq_n);

/**
 * @brief Disables a specific IRQ
 * @param irq_n The IRQ number (0-239)
 */
void nvic_disable_irq(uint8_t irq_n);

/**
 * @brief Sets the priority for a specific IRQ
 * @param irq_n The IRQ number (0-239)
 * @param priority Priority level (0-15 for STM32F407)
 */
void nvic_set_priority(uint8_t irq_n, uint8_t priority);

#endif