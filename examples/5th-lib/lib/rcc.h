#ifndef __RCC_H
#define __RCC_H

#include <stdint.h>
#include "main.h"

// RCC peripheral structure
struct rcc {
    volatile uint32_t CR;            // Clock control register (Internal/External/PLL enable)
    volatile uint32_t PLLCFGR;       // PLL configuration register
    volatile uint32_t CFGR;          // Clock configuration register (Bus prescalers/Source selection)
    volatile uint32_t CIR;           // Clock interrupt register
    volatile uint32_t AHB1RSTR;      // AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;      // AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR;      // AHB3 peripheral reset register
    volatile uint32_t RESERVED0;     // Boundary padding
    volatile uint32_t APB1RSTR;      // APB1 peripheral reset register
    volatile uint32_t APB2RSTR;      // APB2 peripheral reset register
    volatile uint32_t RESERVED1[2];  // Boundary padding
    volatile uint32_t AHB1ENR;       // AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;       // AHB2 peripheral clock enable register
    volatile uint32_t AHB3ENR;       // AHB3 peripheral clock enable register
    volatile uint32_t RESERVED2;     // Boundary padding
    volatile uint32_t APB1ENR;       // APB1 peripheral clock enable register
    volatile uint32_t APB2ENR;       // APB2 peripheral clock enable register
    volatile uint32_t RESERVED3[2];  // Boundary padding
    volatile uint32_t AHB1LPENR;     // AHB1 peripheral clock enable in low power mode
    volatile uint32_t AHB2LPENR;     // AHB2 peripheral clock enable in low power mode
    volatile uint32_t AHB3LPENR;     // AHB3 peripheral clock enable in low power mode
    volatile uint32_t RESERVED4;     // Boundary padding
    volatile uint32_t APB1LPENR;     // APB1 peripheral clock enable in low power mode
    volatile uint32_t APB2LPENR;     // APB2 peripheral clock enable in low power mode
    volatile uint32_t RESERVED5[2];  // Boundary padding
    volatile uint32_t BDCR;          // Backup domain control register (RTC settings)
    volatile uint32_t CSR;           // Control/status register (Reset flags and LSI control)
    volatile uint32_t RESERVED6[2];  // Boundary padding
    volatile uint32_t SSCGR;         // Spread spectrum clock generation register
    volatile uint32_t PLLI2SCFGR;    // PLLI2S configuration register
};

#define RCC ((struct rcc*)0x40023800) // RCC base address

#endif /* __RCC_H */