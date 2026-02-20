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

#include "f4x.h"

// SCB (System Control Block) peripheral structure
struct scb {
    union {
        volatile uint32_t CPUID; // CPUID Base Register
        struct {
            volatile uint32_t REVISION : 4;    // Revision number
            volatile uint32_t PARTNO : 12;     // Part number of the processor
            volatile uint32_t ARCH : 4;        // Architecture (0xF = ARMv7-M)
            volatile uint32_t VARIANT : 4;     // Variant number
            volatile uint32_t IMPLEMENTER : 8; // Implementer code (0x41 = ARM)
        } CPUID_b;
    }; // 0x00
    union {
        volatile uint32_t ICSR; // Interrupt Control and State Register
        struct {
            volatile uint32_t VECTACTIVE : 9;  // Active exception number
            volatile uint32_t : 2;             // Reserved
            volatile uint32_t RETTOBASE : 1;   // Indicates if main stack is the only active stack
            volatile uint32_t VECTPENDING : 6; // Pending exception number
            volatile uint32_t : 4;             // Reserved
            volatile uint32_t ISRPENDING : 1;  // Interrupt pending flag
            volatile uint32_t ISRPREEMPT : 1;  // Pending exception will preempt
            volatile uint32_t : 1;             // Reserved
            volatile uint32_t PENDSTCLR : 1;   // SysTick exception clear-pending bit
            volatile uint32_t PENDSTSET : 1;   // SysTick exception set-pending bit
            volatile uint32_t PENDSVCLR : 1;   // PendSV exception clear-pending bit
            volatile uint32_t PENDSVSET : 1;   // PendSV exception set-pending bit
            volatile uint32_t : 2;             // Reserved
            volatile uint32_t NMIPENDSET : 1;  // NMI set-pending bit
        } ICSR_b;
    }; // 0x04
    union {
        volatile uint32_t VTOR; // Vector Table Offset Register
        struct {
            volatile uint32_t : 9;         // Reserved
            volatile uint32_t TBLOFF : 21; // Vector table base offset field
            volatile uint32_t : 2;         // Reserved
        } field;
    }; // 0x08
    union {
        volatile uint32_t AIRCR; // Application Interrupt and Reset Control Register
        struct {
            volatile uint32_t VECTRESET : 1;     // System reset (Reserved for Debug)
            volatile uint32_t VECTCLRACTIVE : 1; // Clear active state (Reserved for Debug)
            volatile uint32_t SYSRESETREQ : 1;   // System reset request
            volatile uint32_t : 5;               // Reserved
            volatile uint32_t PRIGROUP : 3;      // Interrupt priority grouping field
            volatile uint32_t : 4;               // Reserved
            volatile uint32_t ENDIANNESS : 1;    // Data endianness (0=Little)
            volatile uint32_t VECTKEY : 16;      // Register key (0x05FA)
        } AIRCR_b;
    }; // 0x0C
    union {
        volatile uint32_t SCR; // System Control Register
        struct {
            volatile uint32_t : 1;             // Reserved
            volatile uint32_t SLEEPONEXIT : 1; // Sleep on exit from ISR
            volatile uint32_t SLEEPDEEP : 1;   // Sleep deep (Low power)
            volatile uint32_t : 1;             // Reserved
            volatile uint32_t SEVONPEND : 1;   // Send Event on pending
            volatile uint32_t : 27;            // Reserved
        } SCR_b;
    }; // 0x10
    union {
        volatile uint32_t CCR; // Configuration and Control Register
        struct {
            volatile uint32_t NONBASETHRDENA : 1; // Configures Processor to enter Thread mode
            volatile uint32_t USERSETMPEND : 1;   // User-set-pending enable
            volatile uint32_t : 1;                // Reserved
            volatile uint32_t UNALIGN_TRP : 1;    // Unaligned access trap enable
            volatile uint32_t DIV_0_TRP : 1;      // Divide by zero trap enable
            volatile uint32_t : 3;                // Reserved
            volatile uint32_t BFHFNMIGN : 1;      // BusFault/NMI ignore MSPs
            volatile uint32_t STKALIGN : 1;       // Stack alignment on exception entry
            volatile uint32_t : 22;               // Reserved
        } CCR_b;
    }; // 0x14

    // System Handler Priority Registers
    volatile uint32_t SHPR1; // 0x18 (MemManage, BusFault, UsageFault)
    volatile uint32_t SHPR2; // 0x1C (SVC)
    volatile uint32_t SHPR3; // 0x20 (PendSV, SysTick)

    union {
        volatile uint32_t SHCSR; // System Handler Control and State Register
        struct {
            volatile uint32_t MEMFAULTACT : 1;    // MemManage fault active
            volatile uint32_t BUSFAULTACT : 1;    // BusFault active
            volatile uint32_t : 1;                // Reserved
            volatile uint32_t USGFAULTACT : 1;    // UsageFault active
            volatile uint32_t : 3;                // Reserved
            volatile uint32_t SVCALLACT : 1;      // SVC call active
            volatile uint32_t MONITORACT : 1;     // Debug monitor active
            volatile uint32_t : 1;                // Reserved
            volatile uint32_t PENDSVACT : 1;      // PendSV active
            volatile uint32_t SYSTICKACT : 1;     // SysTick active
            volatile uint32_t USGFAULTPENDED : 1; // UsageFault pending
            volatile uint32_t MEMFAULTPENDED : 1; // MemManage fault pending
            volatile uint32_t BUSFAULTPENDED : 1; // BusFault pending
            volatile uint32_t SVCALLPENDED : 1;   // SVC call pending
            volatile uint32_t MEMFAULTENA : 1;    // MemManage enable
            volatile uint32_t BUSFAULTENA : 1;    // BusFault enable
            volatile uint32_t USGFAULTENA : 1;    // UsageFault enable
            volatile uint32_t : 13;               // Reserved
        } SHCSR_b;
    }; // 0x24
    union {
        volatile uint32_t CFSR; // Configurable Fault Status Register
        struct {
            volatile uint32_t IACCVIOL : 1;    // Instruction access violation (MemManage)
            volatile uint32_t DACCVIOL : 1;    // Data access violation
            volatile uint32_t : 1;             // Reserved
            volatile uint32_t MUNSTKERR : 1;   // Unstacking error
            volatile uint32_t MSTKERR : 1;     // Stacking error
            volatile uint32_t MLSPERR : 1;     // Floating-point lazy state preservation error
            volatile uint32_t : 1;             // Reserved
            volatile uint32_t MMARVALID : 1;   // MMFAR register valid
            volatile uint32_t IBUSERR : 1;     // Instruction bus error (BusFault)
            volatile uint32_t PRECISERR : 1;   // Precise data bus error
            volatile uint32_t IMPRECISERR : 1; // Imprecise data bus error
            volatile uint32_t UNSTKERR : 1;    // Unstacking error
            volatile uint32_t STKERR : 1;      // Stacking error
            volatile uint32_t LSPERR : 1;      // Floating-point lazy state preservation error
            volatile uint32_t : 1;             // Reserved
            volatile uint32_t BFARVALID : 1;   // BFAR register valid
            volatile uint32_t UNDEFINSTR : 1;  // Undefined instruction (UsageFault)
            volatile uint32_t INVSTATE : 1;    // Invalid state
            volatile uint32_t INVPC : 1;       // Invalid PC
            volatile uint32_t NOCP : 1;        // No coprocessor
            volatile uint32_t : 4;             // Reserved
            volatile uint32_t UNALIGNED : 1;   // Unaligned access
            volatile uint32_t DIVBYZERO : 1;   // Divide by zero
            volatile uint32_t : 6;             // Reserved
        } CFSR_b;
    }; // 0x28
    union {
        volatile uint32_t HFSR;
        struct {
            volatile uint32_t : 1;          // Reserved
            volatile uint32_t VECTTBL : 1;  // Vector table hard fault
            volatile uint32_t : 28;         // Reserved
            volatile uint32_t FORCED : 1;   // Forced hard fault
            volatile uint32_t DEBUG_VT : 1; // Debug event
        } HFSR_b;
    }; // 0x2C

    volatile uint32_t DFSR;  // 0x30 Debug Fault Status Register
    volatile uint32_t MMFAR; // 0x34 MemManage Fault Address Register
    volatile uint32_t BFAR;  // 0x38 Bus Fault Address Register
    volatile uint32_t AFSR;  // 0x3C Auxiliary Fault Status Register

    volatile uint32_t RES[18]; // 0x40 to 0x84 padding

    union {
        volatile uint32_t CPACR; // Coprocessor Access Control Register
        struct {
            volatile uint32_t : 20;     // Reserved
            volatile uint32_t CP10 : 2; // Access privileges for coprocessor 10 (FPU)
            volatile uint32_t CP11 : 2; // Access privileges for coprocessor 11 (FPU)
            volatile uint32_t : 8;      // Reserved
        } CPACR_b;
    }; // 0x88
};

#define SCB ((volatile struct scb *)0xE000ED00) // SCB base address

#endif /* __SCB_H */