#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include <stdbool.h> 

#include "f4x.h"

// GPIO peripheral structure
struct gpio {
    volatile uint32_t MODER;    // Port mode register (Input/Output/AF/Analog)
    volatile uint32_t OTYPER;   // Port output type register (Push-pull/Open-drain)
    volatile uint32_t OSPEEDR;  // Port output speed register
    volatile uint32_t PUPDR;    // Port pull-up/pull-down register
    volatile uint32_t IDR;      // Port input data register
    volatile uint32_t ODR;      // Port output data register
    volatile uint32_t BSRR;     // Port bit set/reset register (Atomic pin control)
    volatile uint32_t LCKR;     // Port configuration lock register
    volatile uint32_t AFR[2];   // Alternate function registers (Low/High)
};

#define GPIO(bank) ((struct gpio*)(0x40020000 + 0x400 * (bank))) // GPIO base address

// Enum values are per datasheet: 0, 1, 2, 3
enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG,
};

void gpio_write(uint16_t pin, bool val);
void gpio_set_mode(uint16_t pin, uint8_t mode);

#endif /* __GPIO_H */