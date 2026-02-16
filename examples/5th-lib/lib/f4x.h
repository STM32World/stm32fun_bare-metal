#ifndef __F4X_H
#define __F4X_H 

#include <stdint.h>
#include <stdbool.h> 

#include "gpio.h"

// Bit manipulation macros
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define _weak __attribute__((weak))

#endif /* __F4X_H */