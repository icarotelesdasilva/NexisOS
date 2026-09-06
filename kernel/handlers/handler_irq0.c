

/*
 
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 
 */

#include "../timer/pit.h"

void irq0_handler(void) {
   
    pit_handler(); 
}