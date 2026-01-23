//
// Created by lth on 23/01/2026.
//

#include <stdint.h>

// main
int main (void) {

    uint32_t cnt = 0;

    while (1) {

        ++cnt;

    }
}

// Startup code
__attribute__((noreturn)) void _reset(void) {

    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();

    for (;;) (void) 0; // Infinite loop - should never be reached

}

extern void _estack(void); // Defined in link.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset
};
