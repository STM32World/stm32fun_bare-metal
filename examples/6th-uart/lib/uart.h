/**
 *
 * UART interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef UART_H
#define UART_H  

#include "f4x.h"

struct uart {
    union {
        volatile uint32_t reg;
        struct {
            volatile uint32_t PE      :1;  // Parity error
            volatile uint32_t FE      :1;  // Framing error
            volatile uint32_t NF      :1;  // Noise detected flag
            volatile uint32_t ORE     :1;  // Overrun error
            volatile uint32_t IDLE    :1;  // IDLE line detected
            volatile uint32_t RXNE    :1;  // Read data register not empty
            volatile uint32_t TC      :1;  // Transmission complete
            volatile uint32_t TXE     :1;  // Transmit data register empty
            volatile uint32_t LBD     :1;  // LIN break detection flag
            volatile uint32_t CTS     :1;  // CTS flag
            volatile uint32_t RES     :22; // Reserved
        } field;
    } SR;

    union {
        volatile uint32_t reg;
        struct {
            volatile uint32_t DR      :9;  // Data value
            volatile uint32_t RES     :23; // Reserved
        } field;
    } DR;

    union {
        volatile uint32_t reg;
        struct {
            volatile uint32_t DIV_Fraction :4;  // fraction of USARTDIV
            volatile uint32_t DIV_Mantissa :12; // mantissa of USARTDIV
            volatile uint32_t RES          :16; // Reserved
        } field;
    } BRR;

    union {
        volatile uint32_t reg;
        struct {
            volatile uint32_t SBK     :1;  // Send break
            volatile uint32_t RWU     :1;  // Receiver wakeup
            volatile uint32_t RE      :1;  // Receiver enable
            volatile uint32_t TE      :1;  // Transmitter enable
            volatile uint32_t IDLEIE  :1;  // IDLE interrupt enable
            volatile uint32_t RXNEIE  :1;  // RXNE interrupt enable
            volatile uint32_t TCIE    :1;  // Transmission complete interrupt enable
            volatile uint32_t TXEIE   :1;  // TXE interrupt enable
            volatile uint32_t PEIE    :1;  // PE interrupt enable
            volatile uint32_t PS      :1;  // Parity selection
            volatile uint32_t PCE     :1;  // Parity control enable
            volatile uint32_t WAKE    :1;  // Wakeup method
            volatile uint32_t M       :1;  // Word length
            volatile uint32_t UE      :1;  // USART enable
            volatile uint32_t RES1    :1;  // Reserved
            volatile uint32_t OVER8   :1;  // Oversampling mode
            volatile uint32_t RES2    :16; // Reserved
        } field;
    } CR1;

    union {
        volatile uint32_t reg;
        struct {
            volatile uint32_t ADD     :4;  // Address of the USART node
            volatile uint32_t RES1    :1;  // Reserved
            volatile uint32_t LBDL    :1;  // LIN break detection length
            volatile uint32_t LBDIE   :1;  // LIN break detection interrupt enable
            volatile uint32_t RES2    :1;  // Reserved
            volatile uint32_t LBCL    :1;  // Last bit clock pulse
            volatile uint32_t CPHA    :1;  // Clock phase
            volatile uint32_t CPOL    :1;  // Clock polarity
            volatile uint32_t CLKEN   :1;  // Clock enable
            volatile uint32_t STOP    :2;  // STOP bits
            volatile uint32_t LINEN   :1;  // LIN mode enable
            volatile uint32_t RES3    :17; // Reserved
        } field;
    } CR2;

    union {
        volatile uint32_t reg;
        struct {
            volatile uint32_t EIE     :1;  // Error interrupt enable
            volatile uint32_t IREN    :1;  // IrDA mode enable
            volatile uint32_t IRLP    :1;  // IrDA low-power
            volatile uint32_t HDSEL   :1;  // Half-duplex selection
            volatile uint32_t NACK    :1;  // Smartcard NACK enable
            volatile uint32_t SCEN    :1;  // Smartcard mode enable
            volatile uint32_t DMAR    :1;  // DMA enable receiver
            volatile uint32_t DMAT    :1;  // DMA enable transmitter
            volatile uint32_t RTSE    :1;  // RTS enable
            volatile uint32_t CTSE    :1;  // CTS enable
            volatile uint32_t CTSIE   :1;  // CTS interrupt enable
            volatile uint32_t ONEBIT  :1;  // One bit sample method enable
            volatile uint32_t RES     :20; // Reserved
        } field;
    } CR3;

    union {
        volatile uint32_t reg;
        struct {
            volatile uint32_t PSC     :8;  // Prescaler value
            volatile uint32_t GT      :8;  // Guard time value
            volatile uint32_t RES     :16; // Reserved
        } field;
    } GTPR;
};

#define USART1 ((struct uart*)0x40011000)
#define USART2 ((struct uart*)0x40004400)
#define USART3 ((struct uart*)0x40004800)
#define UART4 ((struct uart*)0x40004C00)
#define UART5 ((struct uart*)0x40005000)
#define USART6 ((struct uart*)0x40011400)

void uart_init(struct uart *uart, unsigned long baud);
int uart_read_ready(struct uart *uart);
int uart_write_ready(struct uart *uart);
void uart_write_char(struct uart *uart, char c);
void uart_write_buf(struct uart *uart, char *buf, size_t len);

#endif
