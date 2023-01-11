#include "kernel.h"
#include "descriptor.h"
#define IDT_BASE_ADDR 0x1F000

__asm__ ("jmpl  $0x8,$initialize_reg\n");

void initialize_reg(){
        asm volatile("mov %%ax, %%ds\n\t"
                     "mov %%ax, %%es\n\t"
                     "mov %%ax, %%gs\n\t"
                     "mov %%ax, %%fs\n\t"
                     "mov %%ax, %%ss\n\t"
        ::"a"(KERNEL_DATA_SEL):); //load segment register
        asm volatile("mov %0, %%esp"::"i"(0x7000):); //set esp
        // jump to main
        __asm__ ("jmpl  $0x8,$main\n");
}


/**
 * Construct a gate descriptor
 *
 * @param selector Segment selector
 * @param attribute Attribute of gate descriptor
 * @param offset adress of interrupt handle
 */

GateDescriptor construct_gate_descriptor(selector s, uint8_t attribute, uint32_t offset){
    GateDescriptor d;
    d.offset_1 = (uint16_t)(offset & 0xffff);
    d.offset_2 = (uint16_t)((offset >> 16) & 0xffff);
    d.selector = s;
    d.reserved = 0;
    d.type_attributes = attribute;
    return d;
}

/**
 * Initializing IDT
 */

void initialize_idt(){
    LIDT_Format idt; 
    GateDescriptor* idtp = (GateDescriptor*)IDT_BASE_ADDR;

    idt.base = IDT_BASE_ADDR;
    idt.limit = 256*8-1; //256 interrupts

    idtp[0] = construct_gate_descriptor(KERNEL_CODE_SEL,ATTRIBUTE_INTERRUPT_DESCRIPTOR,(uint32_t)isr0);
    //idtp[33] = construct_gate_descriptor(KERNEL_CODE_SEL,ATTRIBUTE_INTERRUPT_DESCRIPTOR,(uint32_t)isr0);
    __asm__ __volatile__("LIDT %0" :: "m" (idt));
}

void main(){
    // initialize IDT
    initialize_idt();
    
    __asm__ __volatile__ ("hlt");
}