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
                        // ah = 0x2
                        // al = 0x3
                        // cl = 0x2
                        // ch = 0
                        // dh = 0
                        // dl = 0x80
                        // bx = 
                     
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
        print("hello world!\r\n");
        GDTR_FORMAT gdtr = {.base = GDT_BASE_ADDR, .limit = 23};
        readdisk();


     asm volatile("movl %0, %%eax\n\t"
                   "movl %%eax, %%ds"
             : 
             :"i" (KERNEL_CODE_SEL) 
             : "%eax");
    __asm__ __volatile__ ("hlt");
}
