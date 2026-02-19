/**
 *
 * GPIO interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef UART_H
#define UART_H  

#include "f4x.h"

struct uart {
    volatile uint32_t SR;   // Status register
    volatile uint32_t DR;   // Data register
    volatile uint32_t BRR;  // Baud rate register
    volatile uint32_t CR1;  // Control register 1
    volatile uint32_t CR2;  // Control register 2
    volatile uint32_t CR3;  // Control register 3
    volatile uint32_t GTPR; // Guard time and prescaler register
};

#define UART(port) ((struct uart*)(0x40011000 + 0x400 * (port)))  // USART base address

void uart_init(struct uart *uart, unsigned long baud);

#endif