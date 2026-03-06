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

#ifndef SIG25120_H_
#define SIG25120_H_

#include "main.h"

typedef struct {
    int max;
} sig25120_handler;

void sig25120_init(sig25120_handler *sig25120, int max);
int sig25120_write(sig25120_handler *sig25120, int addr, uint8_t *data, int len);

#endif /* SIG25120_H_ */
