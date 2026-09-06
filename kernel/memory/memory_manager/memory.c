/*
 * Nexis Kernel - Memory Management Early Detection
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "memory_learn.h"
#include <stdint.h>

extern int total_entryes;
extern void serial_print(const char *str);
extern void serial_print_hex(uint32_t n);
extern void serial_print_hex64(uint64_t n);

void memory(void) 
{

    int entradas_reais = *(volatile uint16_t *)0x57000;
    
    memory_map *mm = (memory_map *)0x50000;

    if (entradas_reais == 0 || entradas_reais > 30) {
        entradas_reais = 6; 
    }

    for (int i = 0; i < entradas_reais; i++) {
        serial_print("Base: ");
        serial_print_hex64(mm[i].base_address);
        serial_print(" | Size: ");
        serial_print_hex64(mm[i].size);
        serial_print(" | Type: ");
        serial_print_hex(mm[i].type);
        serial_print("\n");
    }
}