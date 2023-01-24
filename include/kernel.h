#include "descriptor.h"

SegmentDescriptor construct_seg_descriptor(uint32_t base_addr, uint32_t limit, uint32_t attribute);
selector install_descriptor(SegmentDescriptor* descriptor);
void panic(char* message);

#define KERNEL_CODE_SEL 0x8
#define KERNEL_DATA_SEL 0x10
#define USER_CODE_SEL 0x1b
#define KERNEL_BASE_ADDR 0x4000

#define hlt() asm volatile("cli\n\t" "hlt\n\t":::)