
/*

 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only 
 
 */

#ifndef INTERRUPS_H
#define INTERRUPS_H
#include <stdint.h>

typedef struct { 

uint32_t ds;
uint32_t edi, esi, ebp, esp, ebx, edx, 	ecx, eax;
uint32_t int_no, err_code;
uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

void isr_handler(registers_t regs);
#endif