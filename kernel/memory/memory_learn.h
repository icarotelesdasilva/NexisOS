#ifndef MEMORY_LEARN
#define MEMORY_LEARN

#include <stdint.h> 

typedef struct __attribute__((packed)) {
    uint64_t base_address; 
    uint64_t size;
    uint32_t type; 
    uint32_t Extended_Attributes;
} memory_map;

void memory(void);
void pmm_init(void);

#endif
