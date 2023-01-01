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

void main(){
        print("hello world!\r\n");
        GDTR_FORMAT gdtr = {.base = GDT_BASE_ADDR, .limit = 23};

     asm volatile("movl %0, %%eax\n\t"
                   "movl %%eax, %%ds"
             : 
             :"i" (KERNEL_CODE_SEL) 
             : "%eax");
    __asm__ __volatile__ ("hlt");
}
