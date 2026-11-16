/**
 *
 * I2C interface for STM32F4xx microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef I2C_H
#define I2C_H

#include "f4x.h"

struct i2c {
    union { // Control register 1
        volatile uint32_t CR1;
        struct {
            volatile uint32_t PE : 1;        // Peripheral enable
            volatile uint32_t SMBUS : 1;     // SMBus mode
            volatile uint32_t : 1;           // Reserved
            volatile uint32_t SMBTYPE : 1;   // SMBus type
            volatile uint32_t ENARP : 1;     // ARP enable
            volatile uint32_t ENPEC : 1;     // PEC enable
            volatile uint32_t ENGC : 1;      // General call enable
            volatile uint32_t NOSTRETCH : 1; // Clock stretching disable (slave mode)
            volatile uint32_t START : 1;     // Start generation
            volatile uint32_t STOP : 1;      // Stop generation
            volatile uint32_t ACK : 1;       // Acknowledge enable
            volatile uint32_t POS : 1;       // Acknowledge/PEC position (for data reception)
            volatile uint32_t PEC : 1;       // Packet error checking
            volatile uint32_t ALERT : 1;     // SMBus alert
            volatile uint32_t : 1;           // Reserved
            volatile uint32_t SWRST : 1;     // Software reset
            volatile uint32_t : 16;          // Reserved
        } CR1_b;
    };
    union { // Control register 2
        volatile uint32_t CR2;
        struct {
            volatile uint32_t FREQ : 6;    // Peripheral clock frequency in MHz (2-42)
            volatile uint32_t : 2;         // Reserved
            volatile uint32_t ITERREN : 1; // Error interrupt enable
            volatile uint32_t ITEVTEN : 1; // Event interrupt enable
            volatile uint32_t ITBUFEN : 1; // Buffer interrupt enable
            volatile uint32_t DMAEN : 1;   // DMA requests enable
            volatile uint32_t LAST : 1;    // DMA last transfer
            volatile uint32_t : 19;        // Reserved
        } CR2_b;
    };
    union { // Own address register 1
        volatile uint32_t OAR1;
        struct {
            volatile uint32_t ADD : 10;    // Interface address (bits 7:1 in 7-bit mode)
            volatile uint32_t : 4;         // Reserved (bit 14 must be kept at 1)
            volatile uint32_t BIT14 : 1;   // Must be kept at 1 by software
            volatile uint32_t ADDMODE : 1; // Addressing mode (0: 7-bit, 1: 10-bit)
            volatile uint32_t : 16;        // Reserved
        } OAR1_b;
    };
    union { // Own address register 2
        volatile uint32_t OAR2;
        struct {
            volatile uint32_t ENDUAL : 1; // Dual addressing mode enable
            volatile uint32_t ADD2 : 7;   // Interface address
            volatile uint32_t : 24;       // Reserved
        } OAR2_b;
    };
    union { // Data register
        volatile uint32_t DR;
        struct {
            volatile uint32_t DR : 8; // 8-bit data register
            volatile uint32_t : 24;   // Reserved
        } DR_b;
    };
    union { // Status register 1 (read only access through SR1_b - see note below)
        volatile uint32_t SR1;
        struct {
            volatile uint32_t SB : 1;       // Start bit (master mode)
            volatile uint32_t ADDR : 1;     // Address sent (master) / matched (slave)
            volatile uint32_t BTF : 1;      // Byte transfer finished
            volatile uint32_t ADD10 : 1;    // 10-bit header sent (master mode)
            volatile uint32_t STOPF : 1;    // Stop detection (slave mode)
            volatile uint32_t : 1;          // Reserved
            volatile uint32_t RXNE : 1;     // Data register not empty (receiver)
            volatile uint32_t TXE : 1;      // Data register empty (transmitter)
            volatile uint32_t BERR : 1;     // Bus error
            volatile uint32_t ARLO : 1;     // Arbitration lost (master mode)
            volatile uint32_t AF : 1;       // Acknowledge failure
            volatile uint32_t OVR : 1;      // Overrun/underrun
            volatile uint32_t PECERR : 1;   // PEC error in reception
            volatile uint32_t : 1;          // Reserved
            volatile uint32_t TIMEOUT : 1;  // Timeout or Tlow error
            volatile uint32_t SMBALERT : 1; // SMBus alert
            volatile uint32_t : 16;         // Reserved
        } SR1_b;
    };
    union { // Status register 2 (read only)
        volatile uint32_t SR2;
        struct {
            volatile uint32_t MSL : 1;        // Master/slave
            volatile uint32_t BUSY : 1;       // Bus busy
            volatile uint32_t TRA : 1;        // Transmitter/receiver
            volatile uint32_t : 1;            // Reserved
            volatile uint32_t GENCALL : 1;    // General call address (slave mode)
            volatile uint32_t SMBDEFAULT : 1; // SMBus device default address (slave mode)
            volatile uint32_t SMBHOST : 1;    // SMBus host header (slave mode)
            volatile uint32_t DUALF : 1;      // Dual flag (slave mode)
            volatile uint32_t PEC : 8;        // Packet error checking register
            volatile uint32_t : 16;           // Reserved
        } SR2_b;
    };
    union { // Clock control register
        volatile uint32_t CCR;
        struct {
            volatile uint32_t CCR : 12; // Clock control in Fm/Sm mode (master mode)
            volatile uint32_t : 2;      // Reserved
            volatile uint32_t DUTY : 1; // Fm mode duty cycle (0: Tlow/Thigh = 2, 1: 16/9)
            volatile uint32_t FS : 1;   // Master mode selection (0: Sm, 1: Fm)
            volatile uint32_t : 16;     // Reserved
        } CCR_b;
    };
    union { // Rise time register
        volatile uint32_t TRISE;
        struct {
            volatile uint32_t TRISE : 6; // Maximum rise time in Fm/Sm mode (master mode)
            volatile uint32_t : 26;      // Reserved
        } TRISE_b;
    };
    union { // Filter register
        volatile uint32_t FLTR;
        struct {
            volatile uint32_t DNF : 4;   // Digital noise filter
            volatile uint32_t ANOFF : 1; // Analog noise filter off
            volatile uint32_t : 27;      // Reserved
        } FLTR_b;
    };
};

#define I2C1 ((volatile struct i2c *)0x40005400) // I2C1 base address (APB1)
#define I2C2 ((volatile struct i2c *)0x40005800) // I2C2 base address (APB1)
#define I2C3 ((volatile struct i2c *)0x40005C00) // I2C3 base address (APB1)

// Status register 1 flags. Most flags are cleared by a software sequence (reading SR1
// followed by SR2 or DR), so SR1/SR2 must never be touched through the bitfields with a
// read-modify-write. The error flags are rc_w0 - clear them with a plain write of ~flag.
#define I2C_SR1_SB BIT(0)    // Start bit
#define I2C_SR1_ADDR BIT(1)  // Address sent
#define I2C_SR1_BTF BIT(2)   // Byte transfer finished
#define I2C_SR1_RXNE BIT(6)  // Data register not empty
#define I2C_SR1_TXE BIT(7)   // Data register empty
#define I2C_SR1_BERR BIT(8)  // Bus error
#define I2C_SR1_ARLO BIT(9)  // Arbitration lost
#define I2C_SR1_AF BIT(10)   // Acknowledge failure
#define I2C_SR1_OVR BIT(11)  // Overrun/underrun
#define I2C_SR1_ERRORS (I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF | I2C_SR1_OVR)

// Status register 2 flags
#define I2C_SR2_MSL BIT(0)  // Master mode
#define I2C_SR2_BUSY BIT(1) // Bus busy

// Control register 1 bits
#define I2C_CR1_SWRST BIT(15) // Software reset

// Clock control register bits
#define I2C_CCR_FS BIT(15) // Fast mode

// Bus speeds for i2c_init()
#define I2C_SPEED_STANDARD 100000   // Standard mode (Sm)
#define I2C_SPEED_FAST 400000       // Fast mode (Fm) - the official maximum for the STM32F4
#define I2C_SPEED_FAST_PLUS 1000000 // Fast mode plus (Fm+) - OUT OF SPEC for the STM32F4, see i2c_init()

// Fast mode plus: the peripheral only starts counting the SCL high time once it sees SCL
// high, so rise time and input filtering stretch every period. This is subtracted from
// the period when calculating the divider. Measured ~240 ns on a Streamline backplane
// with the EEPROM blade (4k7 pull-ups) - a different bus may need a different value.
#define I2C_FAST_PLUS_COMP_NS 240

// Default timeout for each step of a transfer
#define I2C_TIMEOUT_MS 10

// Return codes - 0 is success, negative is an error
enum {
    I2C_OK = 0,
    I2C_ERR_NACK = -1,    // Device did not acknowledge (address or data)
    I2C_ERR_BUS = -2,     // Bus error or arbitration lost
    I2C_ERR_TIMEOUT = -3, // Flag did not appear in time (bus stuck?)
    I2C_ERR_PARAM = -4,   // Invalid argument (unknown instance, bad speed)
    I2C_ERR_SCL_LOW = -5, // Bus recovery: SCL held low (slave stretching forever or short)
    I2C_ERR_SDA_LOW = -6, // Bus recovery: SDA still low after 9 clocks (short or dead slave)
};

// Setup and bus recovery - see i2c.c for details
int i2c_init(volatile struct i2c *i2c, uint16_t scl, uint16_t sda, uint32_t hz);
int i2c_bus_recover(uint16_t scl, uint16_t sda);
int i2c_get_last_recovery(volatile struct i2c *i2c);
uint32_t i2c_get_recovery_count(volatile struct i2c *i2c);
const char *i2c_strerror(int rc);

// Transfers. All return I2C_OK (0) or a negative I2C_ERR_* code; after a timeout or bus
// error the driver frees the bus and re-initializes the peripheral automatically.
bool i2c_probe(volatile struct i2c *i2c, uint8_t addr);
int i2c_write(volatile struct i2c *i2c, uint8_t addr, const uint8_t *data, size_t len);
int i2c_read(volatile struct i2c *i2c, uint8_t addr, uint8_t *data, size_t len);
int i2c_write_mem(volatile struct i2c *i2c, uint8_t addr, uint32_t mem_addr, uint8_t mem_addr_len, const uint8_t *data, size_t len);
int i2c_read_mem(volatile struct i2c *i2c, uint8_t addr, uint32_t mem_addr, uint8_t mem_addr_len, uint8_t *data, size_t len);

#endif /* I2C_H */

// vim: et ts=4 nowrap autoindent
