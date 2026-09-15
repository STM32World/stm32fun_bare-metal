/**
 * 
 * STM32F407 Linker Script
 * 
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 * 
 */

ENTRY(_reset);                                   /* entry point */

MEMORY {
  flash  (rx)  : ORIGIN = 0x08000000, LENGTH = 512k
  ccmram (xrw) : ORIGIN = 0x10000000, LENGTH = 64K
  sram   (rwx) : ORIGIN = 0x20000000, LENGTH = 128k 
}

_estack     = ORIGIN(sram) + LENGTH(sram);       /* stack points to end of SRAM */

SECTIONS {
  .vectors  : { KEEP(*(.vectors)) }   > flash
  .text     : { *(.text*) }           > flash
  .rodata   : { *(.rodata*) }         > flash

  .data : {
    _sdata = .;                                  /* .data section start */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;                                  /* .data section end */
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .ccmram : {
    . = ALIGN(4);
    _sccmram = .;       /* create a global symbol at ccmram start */
    *(.ccmram)
    *(.ccmram*)

    . = ALIGN(4);
    _eccmram = .;       /* create a global symbol at ccmram end */
  } >ccmram AT> flash
  _siccmram = LOADADDR(.ccmram);

  .bss : {
    _sbss = .;                                   /* .bss section start */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;                                   /* .bss section end */
  } > sram

  . = ALIGN(8);
  _end = .;     /* for cmsis_gcc.h  */
}
 
/* vim: et ts=2 nowrap autoindent */