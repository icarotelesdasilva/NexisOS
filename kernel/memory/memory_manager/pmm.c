#include "memory_learn.h"

extern void serial_print(const char *str);
void serial_print_hex64(uint64_t n);
extern void _fim_do_kernel(void);

static uint8_t *pmm_bitmap;

void pmm_init(void)
{
    pmm_bitmap = (uint8_t *)(&_fim_do_kernel);

    int entradas_reais = *(volatile uint16_t *)0x57000;
    memory_map *mm = (memory_map *)0x50000;

    for (int i = 0; i < entradas_reais; i++)
    {
        if (mm[i].type == 1)
        {
            uint64_t paginas = mm[i].size / 0x1000;

            for (uint64_t pagina = 0; pagina < paginas; pagina++)
            {
                uint64_t addr =
                    mm[i].base_address + (pagina * 0x1000);

                uint64_t indice = addr / 0x1000;

                pmm_bitmap[indice] = 0;
            }

            serial_print("Free Memory Region Found:\n");

            serial_print("Base Address: ");
            serial_print_hex64(mm[i].base_address);

            serial_print("\nSize: ");
            serial_print_hex64(mm[i].size);

            serial_print("\n\n");
        }
    }
}

void pmm_allock(void)
{
}