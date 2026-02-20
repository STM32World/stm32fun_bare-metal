/**
 *
 * SysTick (System Timer) interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "f4x.h"

// SysTick peripheral structure
struct systick {
    union { // Control and status register
        volatile uint32_t CSR;
        struct {
            volatile uint32_t ENABLE : 1;    // Counter enable
            volatile uint32_t TICKINT : 1;   // SysTick exception request enable
            volatile uint32_t CLKSOURCE : 1; // Clock source (0: AHB/8, 1: Processor clock)
            volatile uint32_t : 13;          // Reserved
            volatile uint32_t COUNTFLAG : 1; // Returns 1 if timer counted to 0 since last read
            volatile uint32_t : 15;          // Reserved
        } CSR_b;
    };
    union { // Reload value register
        volatile uint32_t RVR;
        struct {
            volatile uint32_t RELOAD : 24; // Value to load into the CVR when it reaches 0
            volatile uint32_t : 8;         // Reserved
        } RVR_b;
    };
    union { // Current value register
        volatile uint32_t CVR;
        struct {
            volatile uint32_t CURRENT : 24; // Current counter value
            volatile uint32_t : 8;          // Reserved
        } CVR_b;
    };
    union { // Calibration value register
        volatile uint32_t CALIB;
        struct {
            volatile uint32_t TENMS : 24; // Calibration value for 10ms
            volatile uint32_t : 6;        // Reserved
            volatile uint32_t SKEW : 1;   // Is calibration value exact?
            volatile uint32_t NOREF : 1;  // Is there a reference clock?
        } CALIB_b;
    };
};

#define SYSTICK ((volatile struct systick *)0xE000E010) // SysTick base address

extern volatile uint32_t s_ticks;

void systick_init(uint32_t ticks);

#endif // _SYSTICK_H_