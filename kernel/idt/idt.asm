
 ; Nexis Kernel
 ; Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 ; Author: @icarotelesdasilva
 ; SPDX-License-Identifier: GPL-2.0-only
 

global idt_load
extern idtp 

section .text
idt_load:
    lidt [idtp]
    ret