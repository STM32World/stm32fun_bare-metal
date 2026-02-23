/**
 *
 * RCC (Reset and Clock Control) interface for STM32 microcontrollers.
 * Verified for STM32F405/407/415/417.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef __RCC_H
#define __RCC_H

#include "f4x.h"

/**
 * @brief RCC Peripheral Register Structure
 * Base address: 0x40023800
 */
struct rcc {
    union {
        volatile uint32_t CR; // 0x00: Clock control register
        struct {
            volatile uint32_t HSION : 1;     // Bit 0: Internal high-speed clock enable
            volatile uint32_t HSIRDY : 1;    // Bit 1: Internal high-speed clock ready flag
            volatile uint32_t : 1;           // Bit 2: Reserved
            volatile uint32_t HSITRIM : 5;   // Bits 3-7: Internal high-speed clock trimming
            volatile uint32_t HSICAL : 8;    // Bits 8-15: Internal high-speed clock calibration
            volatile uint32_t HSEON : 1;     // Bit 16: External high-speed clock enable
            volatile uint32_t HSERDY : 1;    // Bit 17: External high-speed clock ready flag
            volatile uint32_t HSEBYP : 1;    // Bit 18: External high-speed clock bypass
            volatile uint32_t CSSON : 1;     // Bit 19: Clock security system enable
            volatile uint32_t : 4;           // Bits 20-23: Reserved
            volatile uint32_t PLLON : 1;     // Bit 24: Main PLL enable
            volatile uint32_t PLLRDY : 1;    // Bit 25: Main PLL clock ready flag
            volatile uint32_t PLLI2SON : 1;  // Bit 26: PLLI2S enable
            volatile uint32_t PLLI2SRDY : 1; // Bit 27: PLLI2S clock ready flag
            volatile uint32_t : 4;           // Bits 28-31: Reserved
        } CR_b;
    };

    union {
        volatile uint32_t PLLCFGR; // 0x04: PLL configuration register
        struct {
            volatile uint32_t PLLM : 6;   // Bits 0-5: Division factor for the main PLL (M)
            volatile uint32_t PLLN : 9;   // Bits 6-14: Main PLL multiplication factor for VCO (N)
            volatile uint32_t : 1;        // Bit 15: Reserved
            volatile uint32_t PLLP : 2;   // Bits 16-17: Main PLL division factor for system clock (P)
            volatile uint32_t : 4;        // Bits 18-21: Reserved
            volatile uint32_t PLLSRC : 1; // Bit 22: Main PLL/PLLI2S entry clock source (0=HSI, 1=HSE)
            volatile uint32_t : 1;        // Bit 23: Reserved
            volatile uint32_t PLLQ : 4;   // Bits 24-27: Main PLL division factor for USB OTG FS, SDIO, RNG (Q)
            volatile uint32_t : 4;        // Bits 28-31: Reserved
        } PLLCFGR_b;
    };

    union {
        volatile uint32_t CFGR; // 0x08: Clock configuration register
        struct {
            volatile uint32_t SW : 2;      // Bits 0-1: System clock switch (00=HSI, 01=HSE, 10=PLL)
            volatile uint32_t SWS : 2;     // Bits 2-3: System clock switch status (Read-only)
            volatile uint32_t HPRE : 4;    // Bits 4-7: AHB prescaler
            volatile uint32_t : 2;         // Bits 8-9: Reserved
            volatile uint32_t PPRE1 : 3;   // Bits 10-12: APB Low speed prescaler (APB1)
            volatile uint32_t PPRE2 : 3;   // Bits 13-15: APB high-speed prescaler (APB2)
            volatile uint32_t RTCPRE : 5;  // Bits 16-20: HSE division factor for RTC clock
            volatile uint32_t MCO1 : 2;    // Bits 21-22: Microcontroller clock output 1
            volatile uint32_t I2SSRC : 1;  // Bit 23: I2S clock selection
            volatile uint32_t MCO1PRE : 3; // Bits 24-26: MCO1 prescaler
            volatile uint32_t MCO2PRE : 3; // Bits 27-29: MCO2 prescaler
            volatile uint32_t MCO2 : 2;    // Bits 30-31: Microcontroller clock output 2
        } CFGR_b;
    };

    union {
        volatile uint32_t CIR; // 0x0C: Clock interrupt register
        struct {
            volatile uint32_t LSIRDYF : 1;     // Bit 0: LSI ready interrupt flag
            volatile uint32_t HSERDYF : 1;     // Bit 1: HSE ready interrupt flag
            volatile uint32_t HSIRDYF : 1;     // Bit 2: HSI ready interrupt flag
            volatile uint32_t PLLRDYF : 1;     // Bit 3: Main PLL ready interrupt flag
            volatile uint32_t PLLI2SRDYF : 1;  // Bit 4: PLLI2S ready interrupt flag
            volatile uint32_t CSSF : 1;        // Bit 5: Clock security system interrupt flag
            volatile uint32_t : 2;             // Bits 6-7: Reserved
            volatile uint32_t LSIRDYIE : 1;    // Bit 8: LSI ready interrupt enable
            volatile uint32_t HSERDYIE : 1;    // Bit 9: HSE ready interrupt enable
            volatile uint32_t HSIRDYIE : 1;    // Bit 10: HSI ready interrupt enable
            volatile uint32_t PLLRDYIE : 1;    // Bit 11: Main PLL ready interrupt enable
            volatile uint32_t PLLI2SRDYIE : 1; // Bit 12: PLLI2S ready interrupt enable
            volatile uint32_t : 3;             // Bits 13-15: Reserved
            volatile uint32_t LSIRDYC : 1;     // Bit 16: LSI ready interrupt clear
            volatile uint32_t HSERDYC : 1;     // Bit 17: HSE ready interrupt clear
            volatile uint32_t HSIRDYC : 1;     // Bit 18: HSI ready interrupt clear
            volatile uint32_t PLLRDYC : 1;     // Bit 19: Main PLL ready interrupt clear
            volatile uint32_t PLLI2SRDYC : 1;  // Bit 20: PLLI2S ready interrupt clear
            volatile uint32_t CSSC : 1;        // Bit 21: Clock security system interrupt clear
            volatile uint32_t : 9;             // Bits 22-31: Reserved
        } CIR_b;
    };

    union {
        volatile uint32_t AHB1RSTR; // 0x10: AHB1 peripheral reset register
        struct {
            volatile uint32_t GPIOARST : 1;  // Bit 0: GPIOA reset
            volatile uint32_t GPIOBRST : 1;  // Bit 1: GPIOB reset
            volatile uint32_t GPIOCRST : 1;  // Bit 2: GPIOC reset
            volatile uint32_t GPIODRST : 1;  // Bit 3: GPIOD reset
            volatile uint32_t GPIOERST : 1;  // Bit 4: GPIOE reset
            volatile uint32_t GPIOFRST : 1;  // Bit 5: GPIOF reset
            volatile uint32_t GPIOGRST : 1;  // Bit 6: GPIOG reset
            volatile uint32_t GPIOHRST : 1;  // Bit 7: GPIOH reset
            volatile uint32_t GPIOIRST : 1;  // Bit 8: GPIOI reset
            volatile uint32_t : 3;           // Bits 9-11: Reserved
            volatile uint32_t CRCRST : 1;    // Bit 12: CRC reset
            volatile uint32_t : 8;           // Bits 13-20: Reserved
            volatile uint32_t DMA1RST : 1;   // Bit 21: DMA1 reset
            volatile uint32_t DMA2RST : 1;   // Bit 22: DMA2 reset
            volatile uint32_t : 2;           // Bits 23-24: Reserved
            volatile uint32_t ETHMACRST : 1; // Bit 25: Ethernet MAC reset
            volatile uint32_t : 1;           // Bit 26: Reserved
            volatile uint32_t OTGHRST : 1;   // Bit 27: USB OTG HS reset
            volatile uint32_t : 1;           // Bit 28: Reserved
        } AHB1RSTR_b;
    };

    union {
        volatile uint32_t AHB2RSTR; // 0x14: AHB2 peripheral reset register
        struct {
            volatile uint32_t DCMIRST : 1;  // Bit 0: DCMI reset
            volatile uint32_t : 6;          // Bits 1-6: Reserved
            volatile uint32_t CRYPRST : 1;  // Bit 7: CRYP reset
            volatile uint32_t HASHRST : 1;  // Bit 8: HASH reset
            volatile uint32_t RNGRST : 1;   // Bit 9: RNG reset
            volatile uint32_t OTGFSRST : 1; // Bit 10: USB OTG FS reset
            volatile uint32_t : 21;         // Bits 11-31: Reserved
        } AHB2RSTR_b;
    };

    union {
        volatile uint32_t AHB3RSTR; // 0x18: AHB3 peripheral reset register
        struct {
            volatile uint32_t FSMC : 1; // Bit 0: FSMC reset
            volatile uint32_t : 31;     // Bits 1-31: Reserved
        } AHB3RSTR_b;
    };

    uint32_t RESERVED0; // 0x1C: Padding

    union {
        volatile uint32_t APB1RSTR; // 0x20: APB1 peripheral reset register
        struct {
            volatile uint32_t TIM2RST : 1;  // Bit 0: Timer 2 reset
            volatile uint32_t TIM3RST : 1;  // Bit 1: Timer 3 reset
            volatile uint32_t TIM4RST : 1;  // Bit 2: Timer 4 reset
            volatile uint32_t TIM5RST : 1;  // Bit 3: Timer 5 reset
            volatile uint32_t TIM6RST : 1;  // Bit 4: Timer 6 reset
            volatile uint32_t TIM7RST : 1;  // Bit 5: Timer 7 reset
            volatile uint32_t TIM12RST : 1; // Bit 6: Timer 12 reset
            volatile uint32_t TIM13RST : 1; // Bit 7: Timer 13 reset
            volatile uint32_t TIM14RST : 1; // Bit 8: Timer 14 reset
            volatile uint32_t : 2;          // Bits 9-10: Reserved
            volatile uint32_t WWDGRST : 1;  // Bit 11: Window watchdog reset
            volatile uint32_t : 2;          // Bits 12-13: Reserved
            volatile uint32_t SPI2RST : 1;  // Bit 14: SPI2 reset
            volatile uint32_t SPI3RST : 1;  // Bit 15: SPI3 reset
            volatile uint32_t : 1;          // Bit 16: Reserved
            volatile uint32_t UART2RST : 1; // Bit 17: USART2 reset
            volatile uint32_t UART3RST : 1; // Bit 18: USART3 reset
            volatile uint32_t UART4RST : 1; // Bit 19: UART4 reset
            volatile uint32_t UART5RST : 1; // Bit 20: UART5 reset
            volatile uint32_t I2C1RST : 1;  // Bit 21: I2C1 reset
            volatile uint32_t I2C2RST : 1;  // Bit 22: I2C2 reset
            volatile uint32_t I2C3RST : 1;  // Bit 23: I2C3 reset
            volatile uint32_t : 1;          // Bit 24: Reserved
            volatile uint32_t CAN1RST : 1;  // Bit 25: CAN1 reset
            volatile uint32_t CAN2RST : 1;  // Bit 26: CAN2 reset
            volatile uint32_t : 1;          // Bit 27: Reserved
            volatile uint32_t PWRRST : 1;   // Bit 28: Power interface reset
            volatile uint32_t DACRST : 1;   // Bit 29: DAC reset
            volatile uint32_t UART7RST : 1; // Bit 30: UART7 reset
            volatile uint32_t UART8RST : 1; // Bit 31: UART8 reset
        } APB1RSTR_b;
    };

    union {
        volatile uint32_t APB2RSTR; // 0x24: APB2 peripheral reset register
        struct {
            volatile uint32_t TIM1RST : 1;   // Bit 0: Timer 1 reset
            volatile uint32_t TIM8RST : 1;   // Bit 1: Timer 8 reset
            volatile uint32_t : 2;           // Bits 2-3: Reserved
            volatile uint32_t USART1RST : 1; // Bit 4: USART1 reset
            volatile uint32_t USART6RST : 1; // Bit 5: USART6 reset
            volatile uint32_t : 2;           // Bits 6-7: Reserved
            volatile uint32_t ADCRST : 1;    // Bit 8: ADC interface reset (common to all ADCs)
            volatile uint32_t : 2;           // Bits 9-10: Reserved
            volatile uint32_t SDIORST : 1;   // Bit 11: SDIO reset
            volatile uint32_t SPI1RST : 1;   // Bit 12: SPI1 reset
            volatile uint32_t SPI4RST : 1;   // Bit 13: SPI4 reset
            volatile uint32_t SYSCFGRST : 1; // Bit 14: System configuration controller reset
            volatile uint32_t : 1;           // Bit 15: Reserved
            volatile uint32_t TIM9RST : 1;   // Bit 16: Timer 9 reset
            volatile uint32_t TIM10RST : 1;  // Bit 17: Timer 10 reset
            volatile uint32_t TIM11RST : 1;  // Bit 18: Timer 11 reset
            volatile uint32_t : 13;          // Bits 19-31: Reserved
        } APB2RSTR_b;
    };

    uint32_t RESERVED1[2]; // 0x28, 0x2C: Padding

    union {
        volatile uint32_t AHB1ENR; // 0x30: AHB1 peripheral clock enable register
        struct {
            volatile uint32_t GPIOAEN : 1;     // Bit 0: GPIOA clock enable
            volatile uint32_t GPIOBEN : 1;     // Bit 1: GPIOB clock enable
            volatile uint32_t GPIOCEN : 1;     // Bit 2: GPIOC clock enable
            volatile uint32_t GPIODEN : 1;     // Bit 3: GPIOD clock enable
            volatile uint32_t GPIOEEN : 1;     // Bit 4: GPIOE clock enable
            volatile uint32_t GPIOFEN : 1;     // Bit 5: GPIOF clock enable
            volatile uint32_t GPIOGEN : 1;     // Bit 6: GPIOG clock enable
            volatile uint32_t GPIOHEN : 1;     // Bit 7: GPIOH clock enable
            volatile uint32_t GPIOIEN : 1;     // Bit 8: GPIOI clock enable
            volatile uint32_t : 3;             // Bits 9-11: Reserved
            volatile uint32_t CRCEN : 1;       // Bit 12: CRC clock enable
            volatile uint32_t : 5;             // Bits 13-17: Reserved
            volatile uint32_t BKPSRAMEN : 1;   // Bit 18: Backup SRAM interface clock enable
            volatile uint32_t : 1;             // Bit 19: Reserved
            volatile uint32_t CCMDATRAMEN : 1; // Bit 20: CCM data RAM clock enable
            volatile uint32_t DMA1EN : 1;      // Bit 21: DMA1 clock enable
            volatile uint32_t DMA2EN : 1;      // Bit 22: DMA2 clock enable
            volatile uint32_t : 2;             // Bits 23-24: Reserved
            volatile uint32_t ETHMACEN : 1;    // Bit 25: Ethernet MAC clock enable
            volatile uint32_t ETHMACTXEN : 1;  // Bit 26: Ethernet Transmission clock enable
            volatile uint32_t ETHMACRXEN : 1;  // Bit 27: Ethernet Reception clock enable
            volatile uint32_t ETHMACPTPEN : 1; // Bit 28: Ethernet PTP clock enable
            volatile uint32_t OTGHSEN : 1;     // Bit 29: USB OTG HS clock enable
            volatile uint32_t OTGHSULPIEN : 1; // Bit 30: USB OTG HS ULPI clock enable
            volatile uint32_t : 1;             // Bit 31: Reserved
        } AHB1ENR_b;
    };

    union {
        volatile uint32_t AHB2ENR; // 0x34: AHB2 peripheral clock enable register
        struct {
            volatile uint32_t DCMIEN : 1;  // Bit 0: DCMI clock enable
            volatile uint32_t : 5;         // Bits 1-5: Reserved
            volatile uint32_t RNGEN : 1;   // Bit 6: Random number generator clock enable
            volatile uint32_t HASHEN : 1;  // Bit 7: Hash modules clock enable
            volatile uint32_t CRYPEN : 1;  // Bit 8: Cryptographic modules clock enable
            volatile uint32_t OTGFSEN : 1; // Bit 9: USB OTG FS clock enable
            volatile uint32_t : 22;        // Bits 10-31: Reserved
        } AHB2ENR_b;
    };

    union {
        volatile uint32_t AHB3ENR; // 0x38: AHB3 peripheral clock enable register
        struct {
            volatile uint32_t FSMCEN : 1; // Bit 0: FSMC clock enable
            volatile uint32_t : 31;       // Bits 1-31: Reserved
        } AHB3ENR_b;
    };

    uint32_t RESERVED2; // 0x3C: Padding

    union {
        volatile uint32_t APB1ENR; // 0x40: APB1 peripheral clock enable register
        struct {
            volatile uint32_t TIM2EN : 1;  // Bit 0: Timer 2 clock enable
            volatile uint32_t TIM3EN : 1;  // Bit 1: Timer 3 clock enable
            volatile uint32_t TIM4EN : 1;  // Bit 2: Timer 4 clock enable
            volatile uint32_t TIM5EN : 1;  // Bit 3: Timer 5 clock enable
            volatile uint32_t TIM6EN : 1;  // Bit 4: Timer 6 clock enable
            volatile uint32_t TIM7EN : 1;  // Bit 5: Timer 7 clock enable
            volatile uint32_t TIM12EN : 1; // Bit 6: Timer 12 clock enable
            volatile uint32_t TIM13EN : 1; // Bit 7: Timer 13 clock enable
            volatile uint32_t TIM14EN : 1; // Bit 8: Timer 14 clock enable
            volatile uint32_t : 2;         // Bits 9-10: Reserved
            volatile uint32_t WWDGEN : 1;  // Bit 11: Window watchdog clock enable
            volatile uint32_t : 2;         // Bits 12-13: Reserved
            volatile uint32_t SPI2EN : 1;  // Bit 14: SPI2 clock enable
            volatile uint32_t SPI3EN : 1;  // Bit 15: SPI3 clock enable
            volatile uint32_t : 1;         // Bit 16: Reserved
            volatile uint32_t UART2EN : 1; // Bit 17: USART2 clock enable
            volatile uint32_t UART3EN : 1; // Bit 18: USART3 clock enable
            volatile uint32_t UART4EN : 1; // Bit 19: UART4 clock enable
            volatile uint32_t UART5EN : 1; // Bit 20: UART5 clock enable
            volatile uint32_t I2C1EN : 1;  // Bit 21: I2C1 clock enable
            volatile uint32_t I2C2EN : 1;  // Bit 22: I2C2 clock enable
            volatile uint32_t I2C3EN : 1;  // Bit 23: I2C3 clock enable
            volatile uint32_t : 1;         // Bit 24: Reserved
            volatile uint32_t CAN1EN : 1;  // Bit 25: CAN1 clock enable
            volatile uint32_t CAN2EN : 1;  // Bit 26: CAN2 clock enable
            volatile uint32_t : 1;         // Bit 27: Reserved
            volatile uint32_t PWREN : 1;   // Bit 28: Power interface clock enable
            volatile uint32_t DACEN : 1;   // Bit 29: DAC clock enable
            volatile uint32_t UART7EN : 1; // Bit 30: UART7 clock enable
            volatile uint32_t UART8EN : 1; // Bit 31: UART8 clock enable
        } APB1ENR_b;
    };

    union {
        volatile uint32_t APB2ENR; // 0x44: APB2 peripheral clock enable register
        struct {
            volatile uint32_t TIM1EN : 1;   // Bit 0: Timer 1 clock enable
            volatile uint32_t TIM8EN : 1;   // Bit 1: Timer 8 clock enable
            volatile uint32_t : 2;          // Bits 2-3: Reserved
            volatile uint32_t USART1EN : 1; // Bit 4: USART1 clock enable
            volatile uint32_t USART6EN : 1; // Bit 5: USART6 clock enable
            volatile uint32_t : 2;          // Bits 6-7: Reserved
            volatile uint32_t ADC1EN : 1;   // Bit 8: ADC1 clock enable
            volatile uint32_t ADC2EN : 1;   // Bit 9: ADC2 clock enable
            volatile uint32_t ADC3EN : 1;   // Bit 10: ADC3 clock enable
            volatile uint32_t SDIOEN : 1;   // Bit 11: SDIO clock enable
            volatile uint32_t SPI1EN : 1;   // Bit 12: SPI1 clock enable
            volatile uint32_t SPI4EN : 1;   // Bit 13: SPI4 clock enable
            volatile uint32_t SYSCFGEN : 1; // Bit 14: System configuration controller clock enable
            volatile uint32_t : 1;          // Bit 15: Reserved
            volatile uint32_t TIM9EN : 1;   // Bit 16: Timer 9 clock enable
            volatile uint32_t TIM10EN : 1;  // Bit 17: Timer 10 clock enable
            volatile uint32_t TIM11EN : 1;  // Bit 18: Timer 11 clock enable
            volatile uint32_t : 13;         // Bits 19-31: Reserved
        } APB2ENR_b;
    };

    uint32_t RESERVED3[2]; // 0x48, 0x4C: Padding

    volatile uint32_t AHB1LPENR; // 0x50: AHB1 peripheral clock enable in low power mode
    volatile uint32_t AHB2LPENR; // 0x54: AHB2 peripheral clock enable in low power mode
    volatile uint32_t AHB3LPENR; // 0x58: AHB3 peripheral clock enable in low power mode
    uint32_t RESERVED4;          // 0x5C: Padding
    volatile uint32_t APB1LPENR; // 0x60: APB1 peripheral clock enable in low power mode
    volatile uint32_t APB2LPENR; // 0x64: APB2 peripheral clock enable in low power mode
    uint32_t RESERVED5[2];       // 0x68, 0x6C: Padding

    union {
        volatile uint32_t BDCR; // 0x70: Backup domain control register
        struct {
            volatile uint32_t LSEON : 1;  // Bit 0: External low-speed oscillator enable
            volatile uint32_t LSERDY : 1; // Bit 1: External low-speed oscillator ready flag
            volatile uint32_t LSEBYP : 1; // Bit 2: External low-speed oscillator bypass
            volatile uint32_t : 5;        // Bits 3-7: Reserved
            volatile uint32_t RTCSEL : 2; // Bits 8-9: RTC clock source selection (01=LSE, 10=LSI, 11=HSE)
            volatile uint32_t : 5;        // Bits 10-14: Reserved
            volatile uint32_t RTCEN : 1;  // Bit 15: RTC clock enable
            volatile uint32_t BDRST : 1;  // Bit 16: Backup domain software reset
            volatile uint32_t : 15;       // Bits 17-31: Reserved
        } BDCR_b;
    };

    union {
        volatile uint32_t CSR; // 0x74: Control/status register
        struct {
            volatile uint32_t LSION : 1;    // Bit 0: Internal low-speed oscillator enable
            volatile uint32_t LSIRDY : 1;   // Bit 1: Internal low-speed oscillator ready flag
            volatile uint32_t : 22;         // Bits 2-23: Reserved
            volatile uint32_t RMVF : 1;     // Bit 24: Remove reset flag
            volatile uint32_t BORRSTF : 1;  // Bit 25: BOR reset flag
            volatile uint32_t PINRSTF : 1;  // Bit 26: PIN reset flag
            volatile uint32_t PORRSTF : 1;  // Bit 27: POR/PDR reset flag
            volatile uint32_t SFTRSTF : 1;  // Bit 28: Software reset flag
            volatile uint32_t IWDGRSTF : 1; // Bit 29: Independent watchdog reset flag
            volatile uint32_t WWDGRSTF : 1; // Bit 30: Window watchdog reset flag
            volatile uint32_t LPWRRSTF : 1; // Bit 31: Low-power reset flag
        } CSR_b;
    };

    uint32_t RESERVED6[2]; // 0x78, 0x7C: Padding

    volatile uint32_t SSCGR;      // 0x80: Spread spectrum clock generation register
    volatile uint32_t PLLI2SCFGR; // 0x84: PLLI2S configuration register
};

#define RCC ((struct rcc *)0x40023800)

#endif /* __RCC_H */

// vim: et ts=4 nowrap autoindent