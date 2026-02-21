/**
 *
 * Utility macros and common includes for STM32F4xx microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef __F4X_H
#define __F4X_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Bit manipulation macros
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define _weak __attribute__((weak))

#include "flash.h"
#include "gpio.h"
#include "rcc.h"
#include "scb.h"
#include "sysclock.h"
#include "systick.h"
#include "uart.h"

#endif /* __F4X_H */