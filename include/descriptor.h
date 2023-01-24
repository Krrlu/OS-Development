#include <stdint.h>
#define GDT_BASE_ADDR 0x8000
#define ATTRIBUTE_INTERRUPT_DESCRIPTOR (uint8_t)0x8e // interrupt gate, dpl 0, present
#define ATTRIBUTE_TSS_DESCRIPTOR (uint32_t)0x00008900 // tss descriptor, dpl 0, present
#define ATTRIBUTE_DATA_DESCRIPTOR_RING0 (uint32_t)0x00c09200 // data segment, dpl 0, present, 4KB granularity
#define ATTRIBUTE_DATA_DESCRIPTOR_RING3 (uint32_t)0x00c0f200 // data segment, dpl 3, present, 4KB granularity
#define SIZE_TSS 0x67
typedef uint16_t selector;

typedef struct{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) GDTR_Format, LIDT_Format;

typedef struct{
    uint16_t offset_1;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  paramater;       // number of paramaters pass by stack. Only use first 5 bits
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;        // offset bits 16..31
}__attribute__((packed)) GateDescriptor;

typedef struct{
    uint16_t limit_1;           // limit bits 0..15
    uint16_t base_address1;     // base_address bits 0..15
    uint8_t  base_address2;     // base_address bits 16..23
    uint8_t  type_attributes;   // type
    uint8_t  limit_2;           // limit bits 16..19 and other 4 bits are attributes
    uint8_t  base_address3;     // offset bits 24..31
}__attribute__((packed)) SegmentDescriptor;


typedef struct{
    uint16_t previous_task_link;
    uint16_t reserved_0;
    uint32_t esp_0;
    uint16_t ss_0;
    uint16_t reserved_1;
    uint32_t esp_1;
    uint16_t ss_1;
    uint16_t reserved_2;
    uint32_t esp_2;
    uint16_t ss_2;
    uint16_t reserved_3;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
	uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;
    uint16_t reserved_4;
	uint16_t cs;
    uint16_t reserved_5;
	uint16_t ss;
    uint16_t reserved_6;
	uint16_t ds;
    uint16_t reserved_7;
	uint16_t fs;
    uint16_t reserved_8;
	uint16_t gs;
    uint16_t reserved_9;
	uint16_t ldt;
    uint16_t reserved_10;
    uint16_t T; //only use the first bit
	uint16_t io_map_Base_Address;
}__attribute__((packed)) tss;