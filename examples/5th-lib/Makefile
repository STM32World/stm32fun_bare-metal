#
# Copyright (c) 2026 STM32World <lth@stm32world.com>
# See LICENSE for license details.
#
# Makefile for building and flashing firmware for STM32F4 microcontroller
# This Makefile compiles the source code, links it to create an ELF file, converts it to a binary format, and provides a target for flashing the firmware to the device using st-flash.
# Usage:
#   make        # Build the firmware
#   make flash  # Flash the firmware to the device
#   make clean  # Clean the build artifacts	

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

LIBDIR = lib

CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
	-Wformat-truncation -fno-common -Wconversion \
	-g3 -Og -ffunction-sections -fdata-sections -I. -I${LIBDIR} \
	-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)

LDFLAGS ?= -Tf4x.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SRCS = main.c $(wildcard ${LIBDIR}/*.c)
OBJS = $(SRCS:.c=.o)

all: firmware.bin

firmware.elf: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf firmware.*

.PHONY: all clean flash
