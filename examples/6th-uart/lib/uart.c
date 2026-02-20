/**
 *
 * USART/UART driver for STM32F4xx series microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "uart.h"

#define RCC_APB2ENR_USART1EN BIT(4)
#define RCC_APB1ENR_USART2EN BIT(17)
#define RCC_APB1ENR_USART3EN BIT(18)
#define RCC_APB1ENR_UART4EN BIT(19)
#define RCC_APB1ENR_UART5EN BIT(20)
#define RCC_APB2ENR_USART6EN BIT(5)

void uart_init(struct uart *uart, unsigned long baud) {

    uint8_t af = 7; // Default AF7 for USART1-3, will be overridden for UART4-5 and USART6
    uint16_t rx = 0, tx = 0;
    uint32_t pclk = 0;

    // Enable USART/UART clock, determine AF and pins based on uart instance
    if (uart == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        af = 7;            // AF7 for USART1
        rx = PIN('A', 10); // USART1_RX
        tx = PIN('A', 9);  // USART1_TX
        pclk = APB2_FREQUENCY;
    } else if (uart == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        af = 7;           // AF7 for USART2
        rx = PIN('A', 3); // USART2_RX
        tx = PIN('A', 2); // USART2_TX
        pclk = APB1_FREQUENCY;
    } else if (uart == USART3) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        af = 7;            // AF7 for USART3
        rx = PIN('B', 11); // USART3_RX
        tx = PIN('B', 10); // USART3_TX
        pclk = APB1_FREQUENCY;
    } else if (uart == UART4) {
        RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
        af = 8;            // AF8 for USART4
        rx = PIN('C', 11); // USART4_RX
        tx = PIN('C', 10); // USART4_TX
        pclk = APB1_FREQUENCY;
    } else if (uart == UART5) {
        RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
        af = 8;            // AF8 for UART5
        rx = PIN('C', 12); // UART5_RX
        tx = PIN('D', 2);  // UART5_TX
        pclk = APB1_FREQUENCY;
    } else if (uart == USART6) {
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        af = 8;           // AF8 for USART6
        rx = PIN('C', 7); // USART6_RX
        tx = PIN('C', 6); // USART6_TX
        pclk = APB2_FREQUENCY;
    }

    // Configure GPIO pins for alternate function
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(rx, af);
    gpio_set_af(tx, af);

    // Disable USART before configuration
    uart->CR1 = 0; // Clear CR1 to disable USART and reset settings

    // Better Baud Rate calculation for 16x oversampling
    // This handles the 4-bit fraction correctly
    uint32_t div = (pclk + (baud / 2)) / baud;
    uart->BRR = div;

    // This prevents RMW glitches and ensures UE is set with/after RE/TE.
    //    struct uart tmp_uart = {0}; // Create a blank template
    //    tmp_uart.CR1.field.RE = 1;
    //    tmp_uart.CR1.field.TE = 1;
    //    tmp_uart.CR1.field.UE = 1;

    //    uart->CR1.reg = tmp_uart.CR1.reg; // Atomic write to the hardware

    // Enable USART, receiver and transmitter
    // This part is interesting because in order to avoid RMW issues with the UE bit,
    // the fields MUST be set to volatile.
    uart->CR1_b.RE = 1; // Enable receiver
    uart->CR1_b.TE = 1; // Enable transmitter
    uart->CR1_b.UE = 1; // Enable USART
}

int uart_read_ready(struct uart *uart) {
    return uart->SR_b.RXNE; // If RXNE bit is set, data is ready
}

uint8_t uart_read_byte(struct uart *uart) {
    return (uint8_t)(uart->DR & 255);
}

void uart_write_char(struct uart *uart, char ch) {
    while (!uart->SR_b.TXE)
        ; // Wait until TXE bit is set, indicating data register is empty
    uart->DR = (uint32_t)ch;
}

void uart_write_buf(struct uart *uart, char *buf, size_t len) {
    while (len-- > 0)
        uart_write_char(uart, *(uint8_t *)buf++);
}

// vim: et sts=4 sw=4 ts=4