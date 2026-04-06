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

#include "main.h"

// Bit manipulation macros
#define BIT(x) (1UL << (x))                            // Create a bitmask with bit x set
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num)) // Encode pin as (bank << 8) | num, e.g., PIN('C', 13) for PC13
#define PINNO(pin) (pin & 255)                         // Extract pin number (0-15) from encoded pin
#define PINBANK(pin) (pin >> 8)                        // Extract bank number (0 for A, 1 for B, etc.) from encoded pin

#define _weak __attribute__((weak)) // Weak attribute for interrupt handlers that can be overridden

// Include headers for various peripherals and system components
#include "exti.h"
#include "flash.h"
#include "gpio.h"
#include "nvic.h"
#include "rcc.h"
#include "rng.h"
#include "scb.h"
#include "syscfg.h"
#include "sysclock.h"
#include "systick.h"
#include "timer.h"
#include "uart.h"
#include "vectors.h"

#endif /* __F4X_H */

// vim: et ts=4 nowrap autoindent