/*
 *
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 *
 */

char *vga_terminal_mem = (char*)0xB8000;

static int term_col = 0;
static int term_row = 0;

void term_clear(void)
{
    for (int i = 0; i < 80 * 25; i++)
    {
        vga_terminal_mem[i * 2]     = ' ';
        vga_terminal_mem[i * 2 + 1] = 0x07; 
    }

    term_col = 0;
    term_row = 0;
}

void term_backspace(void)
{
    if (term_col == 0)
        return;

    term_col--;

    int position = (term_row * 80 + term_col) * 2;

    vga_terminal_mem[position]     = ' ';
    vga_terminal_mem[position + 1] = 0x07; 
}

void term_print(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            term_col = 0;
            term_row++;
        }
        else
        {
            int position = (term_row * 80 + term_col) * 2;

            vga_terminal_mem[position]     = str[i];
            vga_terminal_mem[position + 1] = 0x07;
            term_col++;

            if (term_col >= 80)
            {
                term_col = 0;
                term_row++;
            }
        }

        if (term_row >= 25)
            term_clear();

        i++;
    }
}
