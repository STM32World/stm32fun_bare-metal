/**
 *
 * Flash memory interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef FLASH_H
#define FLASH_H

#include "f4x.h"

// Flash peripheral structure
struct flash {
    union { // Access control register
        volatile uint32_t ACR;
        struct {
            volatile uint32_t LATENCY : 3; // Latency (wait states)
            volatile uint32_t : 5;         // Reserved
            volatile uint32_t PRFTEN : 1;  // Prefetch enable
            volatile uint32_t ICEN : 1;    // Instruction cache enable
            volatile uint32_t DCEN : 1;    // Data cache enable
            volatile uint32_t ICRST : 1;   // Instruction cache reset
            volatile uint32_t DCRST : 1;   // Data cache reset
            volatile uint32_t : 19;        // Reserved
        } ACR_b;
    };
    union { // Key register
        volatile uint32_t KEYR;
        struct {
            volatile uint32_t KEY : 32; // Flash key
        } KEYR_b;
    };
    union { // Option key register
        volatile uint32_t OPTKEYR;
        struct {
            volatile uint32_t OPTKEY : 32; // Option byte key
        } OPTKEYR_b;
    };
    union { // Status register
        volatile uint32_t SR;
        struct {
            volatile uint32_t EOP : 1;    // End of operation
            volatile uint32_t OPERR : 1;  // Operation error
            volatile uint32_t : 2;        // Reserved
            volatile uint32_t WRPERR : 1; // Write protection error
            volatile uint32_t PGAERR : 1; // Programming alignment error
            volatile uint32_t PGPERR : 1; // Programming parallelism error
            volatile uint32_t PGSERR : 1; // Programming sequence error
            volatile uint32_t : 8;        // Reserved
            volatile uint32_t BSY : 1;    // Busy
            volatile uint32_t : 15;       // Reserved
        } SR_b;
    };
    union { // Control register
        volatile uint32_t CR;
        struct {
            volatile uint32_t PG : 1;    // Bit 0: Programming
            volatile uint32_t SER : 1;   // Bit 1: Sector erase
            volatile uint32_t MER : 1;   // Bit 2: Mass erase
            volatile uint32_t SNB : 4;   // Bits 3-6: Sector number
            volatile uint32_t : 1;       // Bit 7: Reserved
            volatile uint32_t PSIZE : 2; // Bits 8-9: Program size (x8, x16, x32, x64)
            volatile uint32_t : 6;       // Bits 10-15: Reserved
            volatile uint32_t STRT : 1;  // Bit 16: Start erase
            volatile uint32_t : 7;       // Bits 17-23: Reserved
            volatile uint32_t EOPIE : 1; // Bit 24: End of operation interrupt enable
            volatile uint32_t ERRIE : 1; // Bit 25: Error interrupt enable
            volatile uint32_t : 5;       // Bits 26-30: Reserved
            volatile uint32_t LOCK : 1;  // Bit 31: Lock (set to lock, cleared by the unlock sequence)
        } CR_b;
    };
    union { // Option control register
        volatile uint32_t OPTCR;
        struct {
            volatile uint32_t OPTLOCK : 1;    // Bit 0: Option lock
            volatile uint32_t OPTSTRT : 1;    // Bit 1: Option start
            volatile uint32_t BOR_LEV : 2;    // Bits 2-3: BOR reset level
            volatile uint32_t : 1;            // Bit 4: Reserved
            volatile uint32_t WDG_SW : 1;     // Bit 5: Watchdog selection (1 = software)
            volatile uint32_t nRST_STOP : 1;  // Bit 6: No reset generated when entering Stop
            volatile uint32_t nRST_STDBY : 1; // Bit 7: No reset generated when entering Standby
            volatile uint32_t RDP : 8;        // Bits 8-15: Read protection (0xAA = level 0)
            volatile uint32_t nWRP : 12;      // Bits 16-27: Sector write protection (0 = protected)
            volatile uint32_t : 4;            // Bits 28-31: Reserved
        } OPTCR_b;
    };
};

#define FLASH ((volatile struct flash *)0x40023C00)

#endif /* FLASH_H */

// vim: et ts=4 nowrap autoindent
