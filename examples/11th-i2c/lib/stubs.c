/**
 * This file provides stub implementations of system calls that are typically required
 * by the C standard library when running in a bare-metal environment.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include <errno.h>
#include <reent.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* This tells GCC to stop complaining about missing prototypes in this file */
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

/**
 * Close a file - there are no files, so always fails.
 */
int _close(int file) {
    (void)file;
    return -1;
}

/**
 * Report every file descriptor as a character device, so newlib treats it
 * like a terminal.
 */
int _fstat(int file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

/**
 * Every file descriptor is a terminal.
 */
int _isatty(int file) {
    (void)file;
    return 1;
}

/**
 * Seeking is not supported - always reports position 0.
 */
int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

/**
 * Reading from stdin is not implemented - always returns end of file.
 */
int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

/**
 * There is only one "process".
 */
int _getpid(void) { return 1; }

/**
 * Signals are not supported.
 */
int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

/**
 * Called by exit()/abort() - there is nowhere to go, so hang.
 */
void _exit(int status) {
    (void)status;
    while (1)
        (void)0;
}

/**
 * Grow the heap (used by malloc and printf). The heap starts at _end (end of
 * .bss, from the linker script) and grows up towards the stack.
 * @param incr: Number of bytes to add
 * @return Start of the new block, or (void *)-1 with errno = ENOMEM if it would
 *         run into the stack
 */
void *_sbrk(ptrdiff_t incr) {
    extern char _end;
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0)
        heap_end = &_end;

    // Refuse to let the heap grow into the stack
    char *sp;
    __asm__ volatile("mov %0, sp" : "=r"(sp));
    if (heap_end + incr > sp) {
        errno = ENOMEM;
        return (void *)-1;
    }

    prev_heap_end = heap_end;
    heap_end += incr;

    return (void *)prev_heap_end;
}

// vim: et ts=4 nowrap autoindent
