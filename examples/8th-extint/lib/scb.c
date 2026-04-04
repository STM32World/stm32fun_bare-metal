/**
 *
 * SCB (System Control Block) implementation for STM32F4xx.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "scb.h"

/**
 * @brief Enables the Floating Point Unit (FPU).
 * Required for any code using float/double on the STM32F407.
 */
void scb_fpu_enable(void) {
    // Set CP10 and CP11 to 0b11 (Full Access)
    // This is found in the Coprocessor Access Control Register (CPACR)
    SCB->CPACR_b.CP10 = 3;
    SCB->CPACR_b.CP11 = 3;
    
    // Ensure the FPU settings are applied before continuing
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

/**
 * @brief Sets the priority grouping.
 * @param group The group value (0-7). 
 * For example, group 3 (0b011) results in 4 bits of pre-emption priority 
 * and 0 bits of subpriority on the F407.
 */
void scb_set_priority_grouping(uint32_t group) {
    uint32_t reg = SCB->AIRCR;
    
    // Clear the key and the priority group bits
    reg &= ~((0xFFFFUL << 16) | (7UL << 8));
    
    // Prepare new value: Add the VECTKEY (0x05FA) and the new group
    reg |= (0x05FAUL << 16) | ((group & 7UL) << 8);
    
    SCB->AIRCR = reg;
}

/**
 * @brief Requests a system-wide reset.
 */
void scb_system_reset(void) {
    // Set VECTKEY and SYSRESETREQ bit
    SCB->AIRCR = (0x05FAUL << 16) | (SCB->AIRCR & 0x7F0) | (1UL << 2);
    
    // Wait for reset to occur
    while (1) {
        __asm volatile ("nop");
    }
}

/**
 * @brief Enables system fault handlers (Usage, Bus, and MemManage).
 */
void scb_enable_faults(void) {
    SCB->SHCSR_b.MEMFAULTENA = 1;
    SCB->SHCSR_b.BUSFAULTENA = 1;
    SCB->SHCSR_b.USGFAULTENA = 1;
}

// vim: et sts=4 sw=4 ts=4