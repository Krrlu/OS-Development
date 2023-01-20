// reference: https://dc0d32.blogspot.com/2010/06/real-mode-in-c-with-gcc-writing.html
/* XXX these must be at top */
#include "kernel.h"
#include "descriptor.h"
__asm__(".code16gcc\n");
__asm__ ("jmpl  $0, $main\n");


#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __NORETURN  __attribute__((noreturn))

/* BIOS interrupts must be done with inline assembly */
void    __NOINLINE __REGPARM print(const char   *s){
        while(*s){
                __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
                s++;
        }
}
/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/

void static inline readdisk(){
                        // use int 13 BIOS interrupt
                        // ah = 0xff
                        // al = 0x3
                        // cl = 0x2
                        // ch = 0
                        // dh = 0
                        // dl = 0x80
                        // ex:bx = 4000:0000
                        
        asm volatile("mov %0, %%ah\n\t"         
                     "mov %1, %%al\n\t"        
                     "mov %2, %%cl\n\t"         
                     "xor %%ch, %%ch\n\t"      
                     "xor %%dh, %%dh\n\t"       
                     "mov %3, %%dl\n\t"         
                     "mov %4, %%bx\n\t"  
                     "mov %%bx, %%es\n\t"  
                     "xor %%bx, %%bx\n\t"     
                     "int %5"
             : 
             :"i" (0x2), "i" (0x3), "i"(0x2), "i" (0x80), "i" (0x4000), "i"(0x13)
             : "%eax","%ecx","%ebx","%edx");

}

void main(){
        //print("hello world!\r\n");
        GDTR_Format gdtr = {.base = GDT_BASE_ADDR, .limit = 799}; // 100 descroptors

        uint32_t *gdt_pointer = (uint32_t*)(GDT_BASE_ADDR + 0x8); // point to second entry in the GDT

        readdisk(); // read from disk

        *gdt_pointer = 0x0000ffff; //set up kernel code segment descroptor 
        *(gdt_pointer+1) = 0x00cf9800; // base = 0, limit = 4GB

        *(gdt_pointer+2) = 0x0000ffff; //set up kernel data segment descroptor 
        *(gdt_pointer+3) = 0x00cf9200; // base = 0, limit = 4GB

        *(gdt_pointer+4) = 0x0000ffff; //set up user code segment descroptor 
        *(gdt_pointer+5) = 0x00cff800; // base = 0, limit = 4GB privilege leve 3

        __asm__ volatile("lgdt %0" :: "m" (gdtr));

        __asm__ __volatile__ ("cli"); //clear interrupt flag

        //enable A20
        asm volatile("inb %0, %%al\n\t" //in al, 0x92
                     "or %1, %%al\n\t"  //or al, 2
                     "outb %%al, %0;"   //out 0x92, al
             :
             :"i" (0x92), "i"(0x2)
             : "%eax");

        //set PE bit to enable protect mode
        asm volatile("movl %%cr0, %%eax\n\t"
                  "orl %0, %%eax\n\t"
                  "movl %%eax, %%cr0"
             :
             :"i" (0x1)
             : "%eax");
        // jump to kernel
        __asm__ ("jmpl  $0x8,$0x40000\n");
}
