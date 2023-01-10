#include "print.h"
#include "mmu.h"
#include "io.h"
#include "descriptor.h"
#include "isr.h"

#define KERNEL_CODE_SEL 0x8
#define KERNEL_DATA_SEL 0x10
#define KERNEL_BASE_ADDR 0x4000