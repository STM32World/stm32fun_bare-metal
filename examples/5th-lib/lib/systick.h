#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>
#include "f4x.h"

// SysTick peripheral structure
struct systick {
    volatile uint32_t CSR;    // Control and Status Register (Enable/Interrupt/Source/Flag)
    volatile uint32_t RVR;    // Reload Value Register (The start value for the countdown)
    volatile uint32_t CVR;    // Current Value Register (Read to see current time/Write to clear)
    volatile uint32_t CALIB;  // Calibration Value Register
};

#define SYSTICK ((struct systick*)0xe000e010) // SysTick base address

extern volatile uint32_t s_ticks;

void systick_init(uint32_t ticks);

#endif // _SYSTICK_H_