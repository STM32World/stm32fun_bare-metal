/**
 *
 * Vector table and startup code for STM32F4xx microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "f4x.h"

// extern declarations for main and systick_handler to avoid implicit declaration warnings
extern int main(void);
extern void systick_handler(void);

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {  // Naked might cause a warning but is allowed in gcc
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for (long* dst = &_sbss; dst < &_ebss; dst++) *dst = 0;

    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();

    for (;;) (void)0;  // Infinite loop - should never be reached
}

extern void _estack(void);  // Defined in f407.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack,
    _reset,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    systick_handler,
};