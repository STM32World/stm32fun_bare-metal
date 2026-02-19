/**
 *
 * GPIO interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "uart.h"

#define RCC_APB2ENR_USART1EN BIT(4)
#define RCC_APB1ENR_USART2EN BIT(17)
#define RCC_APB1ENR_USART3EN BIT(18)
#define RCC_APB1ENR_UART4EN  BIT(19)
#define RCC_APB1ENR_UART5EN  BIT(20)
#define RCC_APB2ENR_USART6EN BIT(5)

void uart_init(struct uart *uart, unsigned long baud) {

    uint8_t af;
    uint16_t rx = 0, tx = 0;
    uint32_t pclk;

    // Enable USART/UART clock, determine AF and pins based on uart instance
    if (uart == UART(1)) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        af = 7;  // AF7 for USART1
        rx = PIN('A', 10);  // USART1_RX
        tx = PIN('A', 9);   // USART1_TX
        pclk = APB2_FREQUENCY;
    } else if (uart == UART(2)) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        af = 7;  // AF7 for USART2
        rx = PIN('A', 3);   // USART2_RX
        tx = PIN('A', 2);   // USART2_TX
        pclk = APB1_FREQUENCY;
    } else if (uart == UART(3)) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        af = 7;  // AF7 for USART3
        rx = PIN('B', 11);  // USART3_RX
        tx = PIN('B', 10);  // USART3_TX
        pclk = APB1_FREQUENCY; 
    } else if (uart == UART(4)) {
        RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
        af = 8;  // AF8 for UART4
        rx = PIN('C', 11);  // UART4_RX
        tx = PIN('C', 10);  // UART4_TX
        pclk = APB1_FREQUENCY;
    } else if (uart == UART(5)) {
        RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
        af = 8;  // AF8 for UART5
        rx = PIN('C', 12);  // UART5_RX
        tx = PIN('D', 2);   // UART5_TX
        pclk = APB1_FREQUENCY;
    } else if (uart == UART(6)) {
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        af = 8;  // AF8 for USART6
        rx = PIN('C', 7);   // USART6_RX
        tx = PIN('C', 6);   // USART6_TX
        pclk = APB2_FREQUENCY;
    }

    // Configure GPIO pins for alternate function
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(rx, af);
    gpio_set_af(tx, af);

    // Disable USART before configuration
    uart->CR1 = 0;

    // Configure baud rate
    uart->BRR = pclk / baud;

    // Enable USART, transmitter and receiver
    uart->CR1 = BIT(13) | BIT(2) | BIT(3);

}
