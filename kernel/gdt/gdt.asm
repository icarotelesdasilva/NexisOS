global gdt_flush
global tss_flush

gdt_flush:
MOV eax, [esp + 4]
lgdt [eax]
MOV ax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax

    JMP 0x08:.reload_cs

    .reload_cs:
    RET
    
    tss_flush:
     mov ax, 0x28        
    ltr ax              
    ret
