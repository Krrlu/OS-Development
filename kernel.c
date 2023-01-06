#include "kernel.h"
#include "print.h"
__asm__ ("jmpl  $0x8,$initialize\n");

void initialize(){
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
void main(){
    
    print_char('h');
    __asm__ __volatile__ ("hlt");
}