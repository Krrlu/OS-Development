#include <stdint.h>
#define GDT_BASE_ADDR 0x8000
typedef struct GDTR_FORMAT{
    uint16_t limit;
    uint32_t base;
} GDTR_FORMAT;