/*
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "interrupt.h"

extern void kpanic(char *str);

void isr_handler(registers_t regs) {
    switch (regs.int_no) {
        case 0:
            kpanic("Division by zero");
            break;
        default:
            kpanic("Unhandled interrupt/exception");
            break;
    }
}
