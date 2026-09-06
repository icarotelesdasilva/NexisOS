#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_placeholder;

    uint32_t ebx;
    uint32_t edx; 
    uint32_t ecx; 
    uint32_t eax; 
} registers_t;

typedef int32_t (*syscall_func_t)(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

extern void serial_print(const char *str);
extern void serial_print_hex(uint32_t val); 



// Syscall 0: Termina um processo (exemplo)
int32_t sys_exit(uint32_t error_code, uint32_t arg2, uint32_t arg3, uint32_t arg4) {
    serial_print("syslog: processo finalizado com codigo: ");
    return 0; 
}

int32_t sys_serial_write(uint32_t str_ptr, uint32_t length, uint32_t arg3, uint32_t arg4) {
    const char* user_str = (const char*)str_ptr;

    if (user_str == NULL) {
        return -1; 
    }

    serial_print(user_str);
    return 0; // Sucesso
}

int32_t sys_add_numbers(uint32_t a, uint32_t b, uint32_t arg3, uint32_t arg4) {
    return (int32_t)(a + b);
}

static const syscall_func_t syscall_table[] = {
    [0] = sys_exit,
    [1] = sys_serial_write,
    [2] = sys_add_numbers
};

const uint32_t MAX_SYSCALLS = sizeof(syscall_table) / sizeof(syscall_table[0]);

void handler_syscall(registers_t *regs) {
    uint32_t syscall_num = regs->eax;

    if (syscall_num >= MAX_SYSCALLS || syscall_table[syscall_num] == NULL) {
        serial_print("syslog: syscall error - nao encontrada\n");
        regs->eax = (uint32_t)-1;
        return;
    }

    syscall_func_t func = syscall_table[syscall_num];

    int32_t return_value = func(regs->ebx, regs->ecx, regs->edx, regs->esi);

    regs->eax = (uint32_t)return_value;
}
