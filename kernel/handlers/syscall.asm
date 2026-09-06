global syscall_handler
extern handler_syscall
extern syscall_table
extern MAX_SYSCALLS

KERNEL_DATA_SEL equ 0x10

section .text

syscall_handler:
    cli
    cld

    push eax           
    push ecx            
    push edx            
    push ebx            
    
    push esp           
    push ebp            
    push esi           
    push edi            

    push ds             
    push es             
    push fs            
    push gs             

    mov ax, KERNEL_DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    
    cmp eax, [MAX_SYSCALLS]
    jae .invalid_syscall

    push esp
    call handler_syscall
    add esp, 4          

.back_to_user:

    pop gs
    pop fs
    pop es
    pop ds

    pop edi
    pop esi
    pop ebp
    add esp, 4          
    pop ebx
    pop edx
    pop ecx
    
   
    mov eax, [esp]      
    add esp, 4       
    sti
    iret

.invalid_syscall:
    mov dword [esp + 28], -1  
    jmp .back_to_user
