#include "task.h"
#include "print.h"
#include "mmu.h"
#include "io.h"
#include "isr.h"
#define IDT_BASE_ADDR 0x1F000
__asm__ ("jmpl  $0x8,$initialize_reg\n");

static tss kernel_tss;
struct task kernel_task; // kernel_task will always be the head of task link

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
 * kernel panic function
 *
 * @param message message for panic
 */

void panic(char* message){
    print_string("kernel panic: ");
    print_string(message);
    hlt();
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
    d.paramater = 0; 
    d.type_attributes = attribute;
    return d;
}

/**
 * Construct a segment descriptor
 *
 * @param base_addr Base adress of segment
 * @param limit Size of segment
 * @param attribute Bits 32 - 63 of descriptor. Bits that correspond to attributes of descriptor are set
 *                  Bits correspond base and limit are all zero
 */

SegmentDescriptor construct_seg_descriptor(uint32_t base_addr, uint32_t limit, uint32_t attribute){
    SegmentDescriptor d;
    d.limit_1 = (uint16_t) limit & 0xffff;
    d.base_address1 = (uint16_t) base_addr & 0xffff;
    d.base_address2 = (uint8_t) ((base_addr >> 16) & 0xff);
    d.type_attributes = (uint8_t) ((attribute >> 8) & 0xff);
    d.limit_2 = (uint8_t) ((limit >> 16) & 0xf) | (uint8_t) (attribute >> 16);
    return d;
}

/**
 * install a descriptor to GDT
 *
 * @param descriptor Pointer to the descriptor that will add to GDT
 * 
 */

selector install_descriptor(SegmentDescriptor* descriptor){
    GDTR_Format GDTR_buffer = {};
    selector sel;

    asm ("sgdt %0"::"m"(GDTR_buffer):"memory");

    // p point to the second descriptor in GDT 
    char *p = 8 + (char*)GDTR_buffer.base;
    
    // (GDTR_buffer.limit + 1 ) >> 3 = (GDTR_buffer.limit + 1 ) / 8 = maximum number of descriptor in GDT
    int i = 1;
    for(; i < (GDTR_buffer.limit + 1 ) >> 3; i++){
        // check the present flag in descriptor
        if(!(*(p + 5) & 0x80)) goto desfound;
        // next descriptor
        p += 8;
    }
    panic("Not enough space in GDT");

    desfound:

    //add the descriptor to GDT
    *(SegmentDescriptor*) p =  *descriptor;

    //make segment selector
    sel = i << 3; // i is the index of descriptor in GDT

    
    return sel;
}

/**
 * Initializing IDT
 */

void initialize_idt(){
    LIDT_Format idt; 
    GateDescriptor* idtp = (GateDescriptor*)IDT_BASE_ADDR;

    idt.base = IDT_BASE_ADDR;
    idt.limit = 256*8-1; //256 interrupts

    idtp[0] = construct_gate_descriptor(KERNEL_CODE_SEL,ATTRIBUTE_INTERRUPT_DESCRIPTOR,(uint32_t)interrupt_handler_0);
    idtp[6] = construct_gate_descriptor(KERNEL_CODE_SEL,ATTRIBUTE_INTERRUPT_DESCRIPTOR,(uint32_t)interrupt_handler_6);
    idtp[13] = construct_gate_descriptor(KERNEL_CODE_SEL,ATTRIBUTE_INTERRUPT_DESCRIPTOR,(uint32_t)interrupt_handler_6);
    // set the dpl of int 55 to 3, then user program with cpl = 3 can access it
    idtp[55] = construct_gate_descriptor(KERNEL_CODE_SEL,ATTRIBUTE_INTERRUPT_DESCRIPTOR | 0x60,(uint32_t)interrupt_handler_55);
    __asm__ __volatile__("LIDT %0" :: "m" (idt));
    //print_string("IDT initialization complete");
}

/**
 * Initializing TSS for kernel
 */

void initialize_kernel_task(){
    // obtain TSS
    kernel_tss.previous_task_link = 0;

    //save cr3 to tss
    asm ("mov %%cr3, %%eax\n\t"
         "mov %%eax, %0\n\t"
        :"=r"(kernel_tss.cr3)
        ::"%eax"); 

    kernel_tss.T = 0;
 
    SegmentDescriptor t = construct_seg_descriptor((uint32_t)&kernel_tss,SIZE_TSS,ATTRIBUTE_TSS_DESCRIPTOR);
    selector s = install_descriptor(&t);
    asm ("ltr %0"::"r"(s):);

    // create task
    kernel_task.id = 0;
    kernel_task.prev = NULL;
    kernel_task.next = NULL;
    kernel_task.tss_sele = s;
    kernel_task.busy = 1;
    
}

void __attribute__((noreturn)) main(){
    // initialize IDT
    initialize_idt();
    
    initialize_kernel_task();
    create_task(50);
    task_switch();

    print_string("\nkernel terminated");

    hlt();
    for(;;);
}