#include <stdint.h>
#define GDT_BASE_ADDR 0x8000
#define ATTRIBUTE_INTERRUPT_DESCRIPTOR (uint8_t)0x8e // interrupt gate, dpl 0, present

typedef uint16_t selector;

typedef struct{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) GDTR_FORMAT, LIDT_Format;

typedef struct{
    uint16_t offset_1;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  reserved;        // reserved
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;        // offset bits 16..31
}__attribute__((packed)) GateDescriptor;

typedef struct{
    uint16_t limit_1;           // limit bits 0..15
    uint16_t base_address1;     // base_address bits 0..15
    uint8_t  base_address2;     // base_address bits 16..23
    uint8_t  type_attributes;   // type
    uint8_t  limit_2;           // limit bits 16..19 and other 4 bits   attributes
    uint8_t  base_address3;     // offset bits 24..31
}__attribute__((packed)) SegmentDescriptor;