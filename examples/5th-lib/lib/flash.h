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
    volatile uint32_t ACR;      // Access control register
    volatile uint32_t KEYR;     // Key register
    volatile uint32_t OPTKEYR;  // Option key register
    volatile uint32_t SR;       // Status register
    volatile uint32_t CR;       // Control register
    volatile uint32_t OPTCR;    // Option control register
};

#define FLASH ((struct flash*)0x40023C00)  // Flash base address

#endif /* __FLASH_H */