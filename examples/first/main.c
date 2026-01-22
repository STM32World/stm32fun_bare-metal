/**
  * First STM32World Bare Metal example
  *
  * 2026 STM32World
  *
  */

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

extern void _estack(void); // Defined in link.ld

// The _reset jump here on startup
int main(void) {
    // Main application code
    while (true) {
        // Infinite loop
    }
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {

    // Defined in linker script
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    // Clear BSS and initialize DATA
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    // Jump to main
    main();

    // If main returns, loop forever - should never happen
    for (;;) (void) 0; // Infinite loop

}

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset
};
