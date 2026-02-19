/**
 *
 * SCB (System Control Block) interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef __SCB_H
#define __SCB_H

#include "main.h"

// SCB (System Control Block) peripheral structure
struct scb {
    volatile uint32_t CPUID;    // CPU ID base register
    volatile uint32_t ICSR;     // Interrupt control and state register
    volatile uint32_t VTOR;     // Vector table offset register
    volatile uint32_t AIRCR;    // Application interrupt and reset control register
    volatile uint32_t SCR;      // System control register
    volatile uint32_t CCR;      // Configuration and control register
    volatile uint8_t SHP[12];   // System handler priority registers
    volatile uint32_t SHCSR;    // System handler control and state register
    volatile uint32_t CFSR;     // Configurable fault status register
    volatile uint32_t HFSR;     // Hard fault status register
    volatile uint32_t DFSR;     // Debug fault status register
    volatile uint32_t MMFAR;    // MemManage fault address register
    volatile uint32_t BFAR;     // Bus fault address register
    volatile uint32_t AFSR;     // Auxiliary fault status register
    volatile uint32_t RES[12];  // Padding to 0xD8
    volatile uint32_t CPACR;    // Coprocessor access control register (For FPU)
};

#define SCB ((struct scb*)0xE000ED00)

#endif /* __SCB_H */