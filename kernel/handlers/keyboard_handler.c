#include <stddef.h>
#include "../interrupts/io.h"
#include <stdint.h>

extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);
extern void pic_send_eoi(uint8_t irq);
extern void keyboard_isr(void);

extern void term_clear(void);
extern void term_backspace(void);
extern void term_print(char *str);

void keyboard_handler(void);

#define SHELL_BUFFER_SIZE 128

static char shell_buffer[SHELL_BUFFER_SIZE];
static uint32_t shell_index = 0;
static uint8_t extended_scancode = 0;

static void shell_put(char c)
{
    if (shell_index >= SHELL_BUFFER_SIZE - 1)
        return;

    shell_buffer[shell_index] = c;
    shell_index++;

    char str[2];

    str[0] = c;
    str[1] = '\0';

    term_print(str);
}

static void shell_backspace(void)
{
    if (shell_index == 0)
        return;

    shell_index--;

    shell_buffer[shell_index] = '\0';

    term_backspace();
}

static void shell_execute(void)
{
    shell_buffer[shell_index] = '\0';

    term_print("\n");

    if (shell_index == 0)
    {
        shell_index = 0;
        term_print("NexisOS> ");
        return;
    }

    switch (shell_buffer[0])
    {
        case 'h':
            if (shell_index == 4 &&
                shell_buffer[1] == 'e' &&
                shell_buffer[2] == 'l' &&
                shell_buffer[3] == 'p')
            {
                term_print("Available commands:\n");
                term_print("  help\n");
                term_print("  clear\n");
                term_print("  version\n");
            }
            else
            {
                term_print("Unknown command\n");
            }
            break;

        case 'c':
            if (shell_index == 5 &&
                shell_buffer[1] == 'l' &&
                shell_buffer[2] == 'e' &&
                shell_buffer[3] == 'a' &&
                shell_buffer[4] == 'r')
            {
                term_clear();
            }
            else
            {
                term_print("Unknown command\n");
            }
            break;

        case 'v':
            if (shell_index == 7 &&
                shell_buffer[1] == 'e' &&
                shell_buffer[2] == 'r' &&
                shell_buffer[3] == 's' &&
                shell_buffer[4] == 'i' &&
                shell_buffer[5] == 'o' &&
                shell_buffer[6] == 'n')
            {
                term_print("Kernel: v0.8.9 - Stable\n");
                term_print("OS version: v0.0.9 - Unstable\n");
            }
            else
            {
                term_print("Unknown command\n");
            }
            break;

        default:
            term_print("Unknown command\n");
            break;
    }

    shell_index = 0;
    shell_buffer[0] = '\0';

    term_print("NexisK> ");
}

void keyboard_handler(void)
{
    uint8_t scancode = inb(0x60);

    if (scancode == 0xE0)
    {
        extended_scancode = 1;
        pic_send_eoi(1);
        return;
    }

    if (scancode & 0x80)
    {
        extended_scancode = 0;
        pic_send_eoi(1);
        return;
    }

    if (extended_scancode)
    {
        switch (scancode)
        {
            case 0x53:

                break;
        }

        extended_scancode = 0;
        pic_send_eoi(1);
        return;
    }

    switch (scancode)
    {
        case 0x02: shell_put('1'); break;
        case 0x03: shell_put('2'); break;
        case 0x04: shell_put('3'); break;
        case 0x05: shell_put('4'); break;
        case 0x06: shell_put('5'); break;
        case 0x07: shell_put('6'); break;
        case 0x08: shell_put('7'); break;
        case 0x09: shell_put('8'); break;
        case 0x0A: shell_put('9'); break;
        case 0x0B: shell_put('0'); break;
        case 0x0C: shell_put('-'); break;
        case 0x0D: shell_put('='); break;

        case 0x10: shell_put('q'); break;
        case 0x11: shell_put('w'); break;
        case 0x12: shell_put('e'); break;
        case 0x13: shell_put('r'); break;
        case 0x14: shell_put('t'); break;
        case 0x15: shell_put('y'); break;
        case 0x16: shell_put('u'); break;
        case 0x17: shell_put('i'); break;
        case 0x18: shell_put('o'); break;
        case 0x19: shell_put('p'); break;

        case 0x1E: shell_put('a'); break;
        case 0x1F: shell_put('s'); break;
        case 0x20: shell_put('d'); break;
        case 0x21: shell_put('f'); break;
        case 0x22: shell_put('g'); break;
        case 0x23: shell_put('h'); break;
        case 0x24: shell_put('j'); break;
        case 0x25: shell_put('k'); break;
        case 0x26: shell_put('l'); break;

        case 0x2C: shell_put('z'); break;
        case 0x2D: shell_put('x'); break;
        case 0x2E: shell_put('c'); break;
        case 0x2F: shell_put('v'); break;
        case 0x30: shell_put('b'); break;
        case 0x31: shell_put('n'); break;
        case 0x32: shell_put('m'); break;

        case 0x39:
            shell_put(' ');
            break;

        case 0x1C:
            shell_execute();
            break;

        case 0x0E:
            shell_backspace();
            break;
    }

    pic_send_eoi(1);
}
