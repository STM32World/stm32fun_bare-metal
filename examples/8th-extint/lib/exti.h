/**
 *
 * EXTI (External Interrupt/Event Controller) interface for STM32F407.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef EXTI_H
#define EXTI_H

#include "f4x.h"

// Macro to generate bitfields for EXTI registers (0 to 22 lines)
#define EXTI_LINES_BITFIELD     \
    volatile uint32_t TR0 : 1;  \
    volatile uint32_t TR1 : 1;  \
    volatile uint32_t TR2 : 1;  \
    volatile uint32_t TR3 : 1;  \
    volatile uint32_t TR4 : 1;  \
    volatile uint32_t TR5 : 1;  \
    volatile uint32_t TR6 : 1;  \
    volatile uint32_t TR7 : 1;  \
    volatile uint32_t TR8 : 1;  \
    volatile uint32_t TR9 : 1;  \
    volatile uint32_t TR10 : 1; \
    volatile uint32_t TR11 : 1; \
    volatile uint32_t TR12 : 1; \
    volatile uint32_t TR13 : 1; \
    volatile uint32_t TR14 : 1; \
    volatile uint32_t TR15 : 1; \
    volatile uint32_t TR16 : 1; \
    volatile uint32_t TR17 : 1; \
    volatile uint32_t TR18 : 1; \
    volatile uint32_t TR19 : 1; \
    volatile uint32_t TR20 : 1; \
    volatile uint32_t TR21 : 1; \
    volatile uint32_t TR22 : 1; \
    volatile uint32_t : 9;

struct exti {
    union {
        volatile uint32_t IMR; // Interrupt mask register
        struct {
            EXTI_LINES_BITFIELD
        } IMR_b;
    };
    union {
        volatile uint32_t EMR; // Event mask register
        struct {
            EXTI_LINES_BITFIELD
        } EMR_b;
    };
    union {
        volatile uint32_t RTSR; // Rising trigger selection register
        struct {
            EXTI_LINES_BITFIELD
        } RTSR_b;
    };
    union {
        volatile uint32_t FTSR; // Falling trigger selection register
        struct {
            EXTI_LINES_BITFIELD
        } FTSR_b;
    };
    union {
        volatile uint32_t SWIER; // Software interrupt event register
        struct {
            EXTI_LINES_BITFIELD
        } SWIER_b;
    };
    union {
        volatile uint32_t PR; // Pending register
        struct {
            EXTI_LINES_BITFIELD
        } PR_b;
    };
};

#define EXTI ((volatile struct exti *)0x40013C00)

// Function prototypes
void exti_init(uint16_t pin, uint8_t falling, uint8_t rising);
int exti_get_pending(uint16_t pin);
void exti_clear_pending(uint16_t pin);

#endif