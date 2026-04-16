/**
 *
 * I2C interface for STM32 microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef __I2C_H
#define __I2C_H

#include "f4x.h"

struct i2c {
    union { // CR1: Control register 1
        volatile uint32_t CR1;
        struct {
            volatile uint32_t PE : 1;    // Peripheral enable
            volatile uint32_t SMBUS : 1; // SMBus mode
            volatile uint32_t : 1;
            volatile uint32_t SMBTYPE : 1;   // SMBus type
            volatile uint32_t ENARP : 1;     // ARP enable
            volatile uint32_t ENPEC : 1;     // PEC enable
            volatile uint32_t ENGC : 1;      // General call enable
            volatile uint32_t NOSTRETCH : 1; // Clock stretching disable (Slave mode)
            volatile uint32_t START : 1;     // Start generation
            volatile uint32_t STOP : 1;      // Stop generation
            volatile uint32_t ACK : 1;       // Acknowledge enable
            volatile uint32_t POS : 1;       // Acknowledge/PEC Position
            volatile uint32_t PEC : 1;       // Packet error checking
            volatile uint32_t ALERT : 1;     // SMBus alert
            volatile uint32_t : 1;
            volatile uint32_t SWRST : 1; // Software reset
            volatile uint32_t : 16;
        } CR1_b;
    };
    union { // CR2: Control register 2
        volatile uint32_t CR2;
        struct {
            volatile uint32_t FREQ : 6; // Peripheral clock frequency
            volatile uint32_t : 2;
            volatile uint32_t ITERREN : 1; // Error interrupt enable
            volatile uint32_t ITEVTEN : 1; // Event interrupt enable
            volatile uint32_t ITBUFEN : 1; // Buffer interrupt enable
            volatile uint32_t DMAEN : 1;   // DMA requests enable
            volatile uint32_t LAST : 1;    // DMA last transfer
            volatile uint32_t : 19;
        } CR2_b;
    };
    union { // OAR1: Own address register 1
        volatile uint32_t OAR1;
        struct {
            volatile uint32_t ADD0 : 1;
            volatile uint32_t ADD7 : 7;
            volatile uint32_t ADD9 : 2;
            volatile uint32_t : 5;
            volatile uint32_t ADDMODE : 1; // 0: 7-bit, 1: 10-bit
            volatile uint32_t : 16;
        } OAR1_b;
    };
    volatile uint32_t OAR2; // Own address register 2 (Dual addressing mode)
    volatile uint32_t DR;   // Data register
    union {                 // SR1: Status register 1
        volatile uint32_t SR1;
        struct {
            volatile uint32_t SB : 1;    // Start bit (Master mode)
            volatile uint32_t ADDR : 1;  // Address sent (Master) / matched (Slave)
            volatile uint32_t BTF : 1;   // Byte transfer finished
            volatile uint32_t ADD10 : 1; // 10-bit header sent (Master)
            volatile uint32_t STOPF : 1; // Stop detection (Slave)
            volatile uint32_t : 1;
            volatile uint32_t RXNE : 1;   // Data register not empty (receivers)
            volatile uint32_t TXE : 1;    // Data register empty (transmitters)
            volatile uint32_t BERR : 1;   // Bus error
            volatile uint32_t ARLO : 1;   // Arbitration lost (Master)
            volatile uint32_t AF : 1;     // Acknowledge failure
            volatile uint32_t OVR : 1;    // Overrun/Underrun
            volatile uint32_t PECERR : 1; // PEC error in reception
            volatile uint32_t : 1;
            volatile uint32_t TIMEOUT : 1;  // Timeout or Tlow error
            volatile uint32_t SMBALERT : 1; // SMBus alert
            volatile uint32_t : 16;
        } SR1_b;
    };
    union { // SR2: Status register 2
        volatile uint32_t SR2;
        struct {
            volatile uint32_t MSL : 1;  // Master/slave
            volatile uint32_t BUSY : 1; // Bus busy
            volatile uint32_t TRA : 1;  // Transmitter/receiver
            volatile uint32_t : 1;
            volatile uint32_t GENCALL : 1; // General call address (Slave)
            volatile uint32_t SMBDEFAULT : 1;
            volatile uint32_t SMBHOST : 1;
            volatile uint32_t DUALF : 1; // Dual flag (Slave)
            volatile uint32_t PEC : 8;   // Packet error checking register
            volatile uint32_t : 16;
        } SR2_b;
    };
    union { // CCR: Clock control register
        volatile uint32_t CCR;
        struct {
            volatile uint32_t CCR_val : 12; // Clock control register value
            volatile uint32_t : 2;
            volatile uint32_t DUTY : 1; // Fast mode duty cycle (0: 2, 1: 16/9)
            volatile uint32_t FS : 1;   // I2C master mode selection (0: SM, 1: FM)
            volatile uint32_t : 16;
        } CCR_b;
    };
    volatile uint32_t TRISE; // TRISE register
    volatile uint32_t FLTR;  // FLTR register (Analog filter)
};

#define I2C1 ((volatile struct i2c *)0x40005400)
#define I2C2 ((volatile struct i2c *)0x40005800)
#define I2C3 ((volatile struct i2c *)0x40005C00)

void i2c_init(volatile struct i2c *i2c, uint32_t baudrate);
void i2c_start(volatile struct i2c *i2c);
void i2c_stop(volatile struct i2c *i2c);
void i2c_write(volatile struct i2c *i2c, uint8_t data);
uint8_t i2c_read_ack(volatile struct i2c *i2c);
uint8_t i2c_read_nack(volatile struct i2c *i2c);
void i2c_send_address(volatile struct i2c *i2c, uint8_t addr, bool read);
bool i2c_is_ready(volatile struct i2c *i2c, uint8_t address);

#endif