/*
 *
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 *
 */

char *vga = (char*)0xB8000;

static int cursor_column = 0;
static int cursor_row = 0;

void vga_clear(void)
{
    for (int i = 0; i < 80 * 25; i++)
    {
        vga[i * 2]     = ' ';
        vga[i * 2 + 1] = 0x07;
    }

    cursor_column = 0;
    cursor_row = 0;
}

void vga_backspace(void)
{
    if (cursor_column == 0)
        return;

    cursor_column--;

    int position = (cursor_row * 80 + cursor_column) * 2;

    vga[position]     = ' ';
    vga[position + 1] = 0x07;
}

void vga_print(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            cursor_column = 0;
            cursor_row++;
        }
        else
        {
            int position = (cursor_row * 80 + cursor_column) * 2;

            vga[position]     = str[i];
            vga[position + 1] = 0x03;

            cursor_column++;

            if (cursor_column >= 80)
            {
                cursor_column = 0;
                cursor_row++;
            }
        }

        if (cursor_row >= 25)
            vga_clear();

        i++;
    }
}   