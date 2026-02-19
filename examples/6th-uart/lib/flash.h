/**
 *
 * Flash memory interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef __FLASH_H
#define __FLASH_H

#include <main.h>

// Flash peripheral structure
struct flash {
    union { // Access control register
        volatile uint32_t reg;
        struct {
            volatile uint32_t LATENCY : 3; // Latency (wait states)
            volatile uint32_t RES1 : 5;    // Reserved
            volatile uint32_t PRFTEN : 1;  // Prefetch enable
            volatile uint32_t ICEN : 1;    // Instruction cache enable
            volatile uint32_t DCEN : 1;    // Data cache enable
            volatile uint32_t ICRST : 1;   // Instruction cache reset
            volatile uint32_t DCRST : 1;   // Data cache reset
            volatile uint32_t RES2 : 19;   // Reserved
        } field;
    } ACR;

    union { // Key register
        volatile uint32_t reg;
        struct {
            volatile uint32_t KEY : 32; // Flash key
        } field;
    } KEYR;

    union { // Option key register
        volatile uint32_t reg;
        struct {
            volatile uint32_t OPTKEY : 32; // Option byte key
        } field;
    } OPTKEYR;

    union { // Status register
        volatile uint32_t reg;
        struct {
            volatile uint32_t EOP : 1;    // End of operation
            volatile uint32_t OPERR : 1;  // Operation error
            volatile uint32_t RES1 : 2;   // Reserved
            volatile uint32_t WRPERR : 1; // Write protection error
            volatile uint32_t PGAERR : 1; // Programming alignment error
            volatile uint32_t PGPERR : 1; // Programming parallelism error
            volatile uint32_t PGSERR : 1; // Programming sequence error
            volatile uint32_t RES2 : 8;   // Reserved
            volatile uint32_t BSY : 1;    // Busy
            volatile uint32_t RES3 : 15;  // Reserved
        } field;
    } SR;

    union { // Control register
        volatile uint32_t reg;
        struct {
            volatile uint32_t PG : 1;    // Programming
            volatile uint32_t SER : 1;   // Sector Erase
            volatile uint32_t MER : 1;   // Mass Erase
            volatile uint32_t SNB : 4;   // Sector number
            volatile uint32_t PSIZE : 2; // Program size
            volatile uint32_t STRT : 1;  // Start
            volatile uint32_t RES1 : 6;  // Reserved
            volatile uint32_t EOPIE : 1; // End of operation interrupt enable
            volatile uint32_t ERRIE : 1; // Error interrupt enable
            volatile uint32_t RES2 : 9;  // Reserved
            volatile uint32_t LOCK : 1;  // Lock
            volatile uint32_t RES3 : 4;  // Reserved
        } field;
    } CR;

    union { // Option control register
        volatile uint32_t reg;
        struct {
            volatile uint32_t OPTLOCK : 1;    // Option lock
            volatile uint32_t OPTSTRT : 1;    // Option start
            volatile uint32_t BOR_LEV : 2;    // BOR reset level
            volatile uint32_t RES1 : 1;       // Reserved
            volatile uint32_t WDG_SW : 1;     // WDG_SW
            volatile uint32_t nRST_STDBY : 1; // nRST_STDBY
            volatile uint32_t nRST_STOP : 1;  // nRST_STOP
            volatile uint32_t nWRP : 12;      // Write protect
            volatile uint32_t RDP : 8;        // Read protect
            volatile uint32_t RES2 : 4;       // Reserved
        } field;
    } OPTCR;
};

#define FLASH ((volatile struct flash *)0x40023C00)

#endif /* __FLASH_H */