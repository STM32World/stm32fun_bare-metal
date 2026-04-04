/**
 *
 * SYSCFG (System Configuration Controller) interface for STM32F407.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef SYSCFG_H
#define SYSCFG_H

#include "f4x.h"

struct syscfg {
    volatile uint32_t MEMRMP;       // Memory remap register
    volatile uint32_t PMC;          // Peripheral mode configuration register
    
    // External interrupt configuration registers 1-4
    union {
        volatile uint32_t EXTICR[4];
        struct {
            // Mapping for EXTICR1
            volatile uint32_t EXTI0 : 4;
            volatile uint32_t EXTI1 : 4;
            volatile uint32_t EXTI2 : 4;
            volatile uint32_t EXTI3 : 4;
            volatile uint32_t : 16;
            // Mapping for EXTICR2
            volatile uint32_t EXTI4 : 4;
            volatile uint32_t EXTI5 : 4;
            volatile uint32_t EXTI6 : 4;
            volatile uint32_t EXTI7 : 4;
            volatile uint32_t : 16;
            // Mapping for EXTICR3
            volatile uint32_t EXTI8 : 4;
            volatile uint32_t EXTI9 : 4;
            volatile uint32_t EXTI10 : 4;
            volatile uint32_t EXTI11 : 4;
            volatile uint32_t : 16;
            // Mapping for EXTICR4
            volatile uint32_t EXTI12 : 4;
            volatile uint32_t EXTI13 : 4;
            volatile uint32_t EXTI14 : 4;
            volatile uint32_t EXTI15 : 4;
            volatile uint32_t : 16;
        } EXTICR_b;
    };
    volatile uint32_t CMPCR;        // Compensation cell control register
};

#define SYSCFG ((volatile struct syscfg *)0x40013800)

#endif