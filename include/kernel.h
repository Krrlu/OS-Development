#include "print.h"
#include "mmu.h"
#include "io.h"
#include "isr.h"

#define KERNEL_CODE_SEL 0x8
#define KERNEL_DATA_SEL 0x10
#define USER_DATA_SEL 0x1E
#define KERNEL_BASE_ADDR 0x4000

#define hlt() asm volatile("cli\n\t" "hlt\n\t":::)