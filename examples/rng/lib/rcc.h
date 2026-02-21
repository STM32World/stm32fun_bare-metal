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

struct rcc {
    union {
        volatile uint32_t CR;
        struct {
            volatile uint32_t HSION     : 1;  // Bit 0: Internal high-speed clock enable
            volatile uint32_t HSIRDY    : 1;  // Bit 1: Internal high-speed clock ready flag
            volatile uint32_t           : 1;  // Bit 2: Reserved
            volatile uint32_t HSITRIM   : 5;  // Bits 3-7: Internal high-speed clock trimming
            volatile uint32_t HSICAL    : 8;  // Bits 8-15: Internal high-speed clock calibration
            volatile uint32_t HSEON     : 1;  // Bit 16: External high-speed clock enable
            volatile uint32_t HSERDY    : 1;  // Bit 17: External high-speed clock ready flag
            volatile uint32_t HSEBYP    : 1;  // Bit 18: External high-speed clock bypass
            volatile uint32_t CSSON     : 1;  // Bit 19: Clock security system enable
            volatile uint32_t           : 4;  // Bits 20-23: Reserved
            volatile uint32_t PLLON     : 1;  // Bit 24: Main PLL enable
            volatile uint32_t PLLRDY    : 1;  // Bit 25: Main PLL clock ready flag
            volatile uint32_t PLLI2SON  : 1;  // Bit 26: PLLI2S enable
            volatile uint32_t PLLI2SRDY : 1;  // Bit 27: PLLI2S clock ready flag
            volatile uint32_t           : 4;  // Bits 28-31: Reserved
        } CR_b;
    }; // 0x00

    union {
        volatile uint32_t PLLCFGR;
        struct {
            volatile uint32_t PLLM      : 6;  // Bits 0-5: Division factor for the main PLL
            volatile uint32_t PLLN      : 9;  // Bits 6-14: Main PLL multiplication factor for VCO
            volatile uint32_t           : 1;  // Bit 15: Reserved
            volatile uint32_t PLLP      : 2;  // Bits 16-17: Main PLL division factor for system clock
            volatile uint32_t           : 4;  // Bits 18-21: Reserved
            volatile uint32_t PLLSRC    : 1;  // Bit 22: Main PLL entry clock source (0=HSI, 1=HSE)
            volatile uint32_t           : 1;  // Bit 23: Reserved
            volatile uint32_t PLLQ      : 4;  // Bits 24-27: Main PLL division factor for USB/RNG
            volatile uint32_t           : 4;  // Bits 28-31: Reserved
        } PLLCFGR_b;
    }; // 0x04

    union {
        volatile uint32_t CFGR;
        struct {
            volatile uint32_t SW        : 2;  // Bits 0-1: System clock switch
            volatile uint32_t SWS       : 2;  // Bits 2-3: System clock switch status
            volatile uint32_t HPRE      : 4;  // Bits 4-7: AHB prescaler
            volatile uint32_t           : 2;  // Bits 8-9: Reserved
            volatile uint32_t PPRE1     : 3;  // Bits 10-12: APB Low speed prescaler (APB1)
            volatile uint32_t PPRE2     : 3;  // Bits 13-15: APB high-speed prescaler (APB2)
            volatile uint32_t RTCPRE    : 5;  // Bits 16-20: HSE division factor for RTC clock
            volatile uint32_t MCO1      : 2;  // Bits 21-22: Microcontroller clock output 1
            volatile uint32_t I2SSRC    : 1;  // Bit 23: I2S clock selection
            volatile uint32_t MCO1PRE   : 3;  // Bits 24-26: MCO1 prescaler
            volatile uint32_t MCO2PRE   : 3;  // Bits 27-29: MCO2 prescaler
            volatile uint32_t MCO2      : 2;  // Bits 30-31: Microcontroller clock output 2
        } CFGR_b;
    }; // 0x08

    union {
        volatile uint32_t CIR;
        struct {
            volatile uint32_t LSIRDYF   : 1;
            volatile uint32_t HSERDYF   : 1;
            volatile uint32_t HSIRDYF   : 1;
            volatile uint32_t PLLRDYF   : 1;
            volatile uint32_t PLLI2SRDYF: 1;
            volatile uint32_t CSSF      : 1;
            volatile uint32_t           : 2;
            volatile uint32_t LSIRDYIE  : 1;
            volatile uint32_t HSERDYIE  : 1;
            volatile uint32_t HSIRDYIE  : 1;
            volatile uint32_t PLLRDYIE  : 1;
            volatile uint32_t PLLI2SRDYIE:1;
            volatile uint32_t           : 3;
            volatile uint32_t LSIRDYC   : 1;
            volatile uint32_t HSERDYC   : 1;
            volatile uint32_t HSIRDYC   : 1;
            volatile uint32_t PLLRDYC   : 1;
            volatile uint32_t PLLI2SRDYC: 1;
            volatile uint32_t CSSC      : 1;
            volatile uint32_t           : 9;
        } CIR_b;
    }; // 0x0C

    union {
        volatile uint32_t AHB1RSTR;
        struct {
            volatile uint32_t GPIOARST  : 1;
            volatile uint32_t GPIOBRST  : 1;
            volatile uint32_t GPIOCRST  : 1;
            volatile uint32_t GPIODRST  : 1;
            volatile uint32_t GPIOERST  : 1;
            volatile uint32_t GPIOFRST  : 1;
            volatile uint32_t GPIOGRST  : 1;
            volatile uint32_t GPIOHRST  : 1;
            volatile uint32_t GPIOIRST  : 1;
            volatile uint32_t           : 3;
            volatile uint32_t CRCRST    : 1;
            volatile uint32_t           : 8;
            volatile uint32_t DMA1RST   : 1;
            volatile uint32_t DMA2RST   : 1;
            volatile uint32_t           : 2;
            volatile uint32_t ETHMACRST : 1;
            volatile uint32_t           : 1;
            volatile uint32_t OTGHRST   : 1;
            volatile uint32_t           : 1;
        } AHB1RSTR_b;
    }; // 0x10

    union {
        volatile uint32_t AHB2RSTR;
        struct {
            volatile uint32_t DCMIRST   : 1; // Bit 0
            volatile uint32_t           : 6; // Bits 1-6
            volatile uint32_t CRYPRST   : 1; // Bit 7
            volatile uint32_t HASHRST   : 1; // Bit 8
            volatile uint32_t RNGRST    : 1; // Bit 9
            volatile uint32_t OTGFSRST  : 1; // Bit 10
            volatile uint32_t           : 21;
        } AHB2RSTR_b;
    }; // 0x14

    union {
        volatile uint32_t AHB3RSTR;
        struct {
            volatile uint32_t FSMC      : 1;
            volatile uint32_t           : 31;
        } AHB3RSTR_b;
    }; // 0x18

    uint32_t RESERVED0; // 0x1C padding

    union {
        volatile uint32_t APB1RSTR;
        struct {
            volatile uint32_t TIM2RST   : 1;
            volatile uint32_t TIM3RST   : 1;
            volatile uint32_t TIM4RST   : 1;
            volatile uint32_t TIM5RST   : 1;
            volatile uint32_t TIM6RST   : 1;
            volatile uint32_t TIM7RST   : 1;
            volatile uint32_t TIM12RST  : 1;
            volatile uint32_t TIM13RST  : 1;
            volatile uint32_t TIM14RST  : 1;
            volatile uint32_t           : 2;
            volatile uint32_t WWDGRST   : 1;
            volatile uint32_t           : 2;
            volatile uint32_t SPI2RST   : 1;
            volatile uint32_t SPI3RST   : 1;
            volatile uint32_t           : 1;
            volatile uint32_t UART2RST  : 1;
            volatile uint32_t UART3RST  : 1;
            volatile uint32_t UART4RST  : 1;
            volatile uint32_t UART5RST  : 1;
            volatile uint32_t I2C1RST   : 1;
            volatile uint32_t I2C2RST   : 1;
            volatile uint32_t I2C3RST   : 1;
            volatile uint32_t           : 1;
            volatile uint32_t CAN1RST   : 1;
            volatile uint32_t CAN2RST   : 1;
            volatile uint32_t           : 1;
            volatile uint32_t PWRRST    : 1;
            volatile uint32_t DACRST    : 1;
            volatile uint32_t UART7RST  : 1;
            volatile uint32_t UART8RST  : 1;
        } APB1RSTR_b;
    }; // 0x20

    union {
        volatile uint32_t APB2RSTR;
        struct {
            volatile uint32_t TIM1RST   : 1;
            volatile uint32_t TIM8RST   : 1;
            volatile uint32_t           : 2;
            volatile uint32_t USART1RST : 1;
            volatile uint32_t USART6RST : 1;
            volatile uint32_t           : 2;
            volatile uint32_t ADCRST    : 1;
            volatile uint32_t           : 2;
            volatile uint32_t SDIORST   : 1;
            volatile uint32_t SPI1RST   : 1;
            volatile uint32_t SPI4RST   : 1;
            volatile uint32_t SYSCFGRST : 1;
            volatile uint32_t           : 1;
            volatile uint32_t TIM9RST   : 1;
            volatile uint32_t TIM10RST  : 1;
            volatile uint32_t TIM11RST  : 1;
            volatile uint32_t           : 13;
        } APB2RSTR_b;
    }; // 0x24

    uint32_t RESERVED1[2]; // 0x28, 0x2C padding

    union {
        volatile uint32_t AHB1ENR;
        struct {
            volatile uint32_t GPIOAEN   : 1;  // Bit 0
            volatile uint32_t GPIOBEN   : 1;  // Bit 1
            volatile uint32_t GPIOCEN   : 1;  // Bit 2
            volatile uint32_t GPIODEN   : 1;  // Bit 3
            volatile uint32_t GPIOEEN   : 1;  // Bit 4
            volatile uint32_t GPIOFEN   : 1;  // Bit 5
            volatile uint32_t GPIOGEN   : 1;  // Bit 6
            volatile uint32_t GPIOHEN   : 1;  // Bit 7
            volatile uint32_t GPIOIEN   : 1;  // Bit 8
            volatile uint32_t           : 3;  // Bits 9-11: Reserved
            volatile uint32_t CRCEN     : 1;  // Bit 12
            volatile uint32_t           : 5;  // Bits 13-17: Reserved
            volatile uint32_t BKPSRAMEN : 1;  // Bit 18
            volatile uint32_t           : 1;  // Bit 19: Reserved
            volatile uint32_t CCMDATRAMEN: 1; // Bit 20
            volatile uint32_t DMA1EN    : 1;  // Bit 21
            volatile uint32_t DMA2EN    : 1;  // Bit 22
            volatile uint32_t           : 2;  // Bits 23-24: Reserved
            volatile uint32_t ETHMACEN  : 1;  // Bit 25
            volatile uint32_t ETHMACTXEN: 1;  // Bit 26
            volatile uint32_t ETHMACRXEN: 1;  // Bit 27
            volatile uint32_t ETHMACPTPEN: 1; // Bit 28
            volatile uint32_t OTGHSEN   : 1;  // Bit 29
            volatile uint32_t OTGHSULPIEN: 1; // Bit 30
            volatile uint32_t           : 1;  // Bit 31: Reserved
        } AHB1ENR_b;
    }; // 0x30

    union {
        volatile uint32_t AHB2ENR;
        struct {
            volatile uint32_t DCMIEN    : 1;  // Bit 0
            volatile uint32_t           : 5;  // Bits 1-5: Reserved
            volatile uint32_t RNGEN     : 1;  // Bit 6: RNG Clock Enable
            volatile uint32_t HASHEN    : 1;  // Bit 7
            volatile uint32_t CRYPEN    : 1;  // Bit 8
            volatile uint32_t OTGFSEN   : 1;  // Bit 9
            volatile uint32_t           : 22; // Bits 10-31: Reserved
        } AHB2ENR_b;
    }; // 0x34

    union {
        volatile uint32_t AHB3ENR;
        struct {
            volatile uint32_t FSMCEN    : 1;
            volatile uint32_t           : 31;
        } AHB3ENR_b;
    }; // 0x38

    uint32_t RESERVED2; // 0x3C padding

    union {
        volatile uint32_t APB1ENR;
        struct {
            volatile uint32_t TIM2EN    : 1;  // Bit 0
            volatile uint32_t TIM3EN    : 1;
            volatile uint32_t TIM4EN    : 1;
            volatile uint32_t TIM5EN    : 1;
            volatile uint32_t TIM6EN    : 1;
            volatile uint32_t TIM7EN    : 1;
            volatile uint32_t TIM12EN   : 1;
            volatile uint32_t TIM13EN   : 1;
            volatile uint32_t TIM14EN   : 1;
            volatile uint32_t           : 2;
            volatile uint32_t WWDGEN    : 1;  // Bit 11
            volatile uint32_t           : 2;
            volatile uint32_t SPI2EN    : 1;
            volatile uint32_t SPI3EN    : 1;
            volatile uint32_t           : 1;
            volatile uint32_t UART2EN   : 1;
            volatile uint32_t UART3EN   : 1;
            volatile uint32_t UART4EN   : 1;
            volatile uint32_t UART5EN   : 1;
            volatile uint32_t I2C1EN    : 1;
            volatile uint32_t I2C2EN    : 1;
            volatile uint32_t I2C3EN    : 1;
            volatile uint32_t           : 1;
            volatile uint32_t CAN1EN    : 1;
            volatile uint32_t CAN2EN    : 1;
            volatile uint32_t           : 1;
            volatile uint32_t PWREN     : 1;
            volatile uint32_t DACEN     : 1;
            volatile uint32_t UART7EN   : 1;  // Bit 30
            volatile uint32_t UART8EN   : 1;  // Bit 31
        } APB1ENR_b;
    }; // 0x40

    union {
        volatile uint32_t APB2ENR;
        struct {
            volatile uint32_t TIM1EN    : 1;  // Bit 0
            volatile uint32_t TIM8EN    : 1;
            volatile uint32_t           : 2;
            volatile uint32_t USART1EN  : 1;
            volatile uint32_t USART6EN  : 1;
            volatile uint32_t           : 2;
            volatile uint32_t ADC1EN    : 1;
            volatile uint32_t ADC2EN    : 1;
            volatile uint32_t ADC3EN    : 1;
            volatile uint32_t SDIOEN    : 1;
            volatile uint32_t SPI1EN    : 1;
            volatile uint32_t SPI4EN    : 1;
            volatile uint32_t SYSCFGEN  : 1;
            volatile uint32_t           : 1;
            volatile uint32_t TIM9EN    : 1;
            volatile uint32_t TIM10EN   : 1;
            volatile uint32_t TIM11EN   : 1;
            volatile uint32_t           : 13;
        } APB2ENR_b;
    }; // 0x44

    uint32_t RESERVED3[2]; // 0x48, 0x4C padding

    volatile uint32_t AHB1LPENR; // 0x50
    volatile uint32_t AHB2LPENR; // 0x54
    volatile uint32_t AHB3LPENR; // 0x58
    uint32_t RESERVED4;          // 0x5C
    volatile uint32_t APB1LPENR; // 0x60
    volatile uint32_t APB2LPENR; // 0x64
    uint32_t RESERVED5[2];       // 0x68, 0x6C

    union {
        volatile uint32_t BDCR;
        struct {
            volatile uint32_t LSEON     : 1;
            volatile uint32_t LSERDY    : 1;
            volatile uint32_t LSEBYP    : 1;
            volatile uint32_t           : 5;
            volatile uint32_t RTCSEL    : 2;
            volatile uint32_t           : 5;
            volatile uint32_t RTCEN     : 1;
            volatile uint32_t BDRST     : 1;
            volatile uint32_t           : 15;
        } BDCR_b;
    }; // 0x70

    union {
        volatile uint32_t CSR;
        struct {
            volatile uint32_t LSION     : 1;
            volatile uint32_t LSIRDY    : 1;
            volatile uint32_t           : 22;
            volatile uint32_t RMVF      : 1;
            volatile uint32_t BORRSTF   : 1;
            volatile uint32_t PINRSTF   : 1;
            volatile uint32_t PORRSTF   : 1;
            volatile uint32_t SFTRSTF   : 1;
            volatile uint32_t IWDGRSTF  : 1;
            volatile uint32_t WWDGRSTF  : 1;
            volatile uint32_t LPWRRSTF  : 1;
        } CSR_b;
    }; // 0x74

    uint32_t RESERVED6[2]; // 0x78, 0x7C

    volatile uint32_t SSCGR;      // 0x80
    volatile uint32_t PLLI2SCFGR; // 0x84
};

#define RCC ((struct rcc *)0x40023800)

#endif /* __RCC_H */