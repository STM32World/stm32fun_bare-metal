/**
 ******************************************************************************
 * @file           : sig25120.h
 * @brief          : Signetics 25120 STM32 Implementation
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "sig25120.h"

void sig25120_init(sig25120_handler *sig25120, int max) {
    sig25120->max = max;
}

int sig25120_write(sig25120_handler *sig25120, int addr, uint8_t *data, int len) {
    
    int written;

    written = (int)data[0]; // Simulate writing the first byte to the address

    written = addr + len >= sig25120->max ? len - (len - addr) : len;
    return written;
}
