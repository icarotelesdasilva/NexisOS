/*
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <stdint.h>
#include "pmm.h"
#include "interrupts/pic.h"
#include "memory/memory_learn.h"

int total_entryes = 0;

extern void init_gdt(void);

extern void keyboard_handler(void);

extern void serial_print(const char *str);
extern void serial_print_hex(uint32_t *n);

extern void memory(void);
extern void pmm_init(void);

extern void term_clear(void);
extern void term_backspace(void);
extern void term_print(char *str);

extern void vga_clear(void);
extern void vga_print(char *str);

extern void pic_remap(uint8_t offset1, uint8_t offset2);

extern void idt_install(void);

extern void init_pit(uint32_t frequency);

extern void mouse_register_interrupt(void);
extern void ps2_mouse_init(int screen_width, int screen_height);
extern void unmask_mouse_irq(void);

void kmain(void) {
    init_gdt();

    pic_remap(0x20, 0x28);

    idt_install();

    //init_pit(1000);

 /* 

WARNING!!!

These functions were temporarily disabled due to an operating system malfunction, so they were isolated.

*/

//    mouse_register_interrupt();
  //  ps2_mouse_init(800, 600);
    //unmask_mouse_irq();
    keyboard_handler();

    vga_clear();

    serial_print("kernel alive.\n");


    memory();

    pmm_init();

    vga_print("Kernel alive.");
    term_print("\n Press any key or type 'help' for more information.");
    for (;;)
        asm volatile ("sti");
}
