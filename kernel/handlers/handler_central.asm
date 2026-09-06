; Nexis Kernel
; Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
; Author: @icarotelesdasilva
; SPDX-License-Identifier: GPL-2.0-only
 
section .rodata
    double_fault_str db "KERNEL PANIC: Double Fault (0x08)", 0
global irq12_stub
global isr_double_fault
global isr0
global irq0_isr
global keyboard_isr

extern ps2_mouse_handle_interrupt
extern keyboard_handler
extern handler_0x08
extern isr_handler
extern irq0_handler
extern current
extern next

section .text

keyboard_isr:
    pushad

    push ds
    push es

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    call keyboard_handler

    pop es
    pop ds

    popad

    iretd


irq0_isr:
    pusha
    push ds
    push es

    mov eax, [current]
    mov [eax + 4], esp

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    call irq0_handler

    mov eax, [next]
    mov esp, [eax + 4]

    pop es
    pop ds
    popa
    iret

isr_double_fault:
    cli
    add esp, 4             
    push double_fault_str 
    call handler_0x08

.hang:
    hlt
    jmp .hang

isr0:
    push byte 0         
    push byte 0         
    pusha               
    mov ax, ds          
    push eax
    call isr_handler   
    pop eax
    popa                
    add esp, 8          
    iret

[bits 32]
irq12_stub:
    pusha                      

    push dword 600              
    push dword 800              
    call ps2_mouse_handle_interrupt
    add esp, 8                  

    mov al, 0x20
    out 0xA0, al
    out 0x20, al

    popa                       
    iret                   