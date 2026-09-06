#include <stdint.h>

extern void kpanic(const char *str);

void handler_0x08(const char* msg) {

kpanic(msg);

}	