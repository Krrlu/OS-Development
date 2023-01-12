#include "print.h"
#include "isr.h"
/*
    This file contain Interrupt Service Routine
*/
__attribute__((interrupt)) void interrupt_handler_0(interrupt_frame* frame){
    print_string("Error, divide by zero");
    __asm__ __volatile__ ("hlt");
}

__attribute__((interrupt)) void interrupt_handler_6(interrupt_frame* frame){
    print_string("Error, invalid Opcode");
    __asm__ __volatile__ ("hlt");
}

__attribute__((interrupt)) void interrupt_handler_13(interrupt_frame* frame){
    print_string("error, general protection");
    __asm__ __volatile__ ("hlt");
}

/**
* A Handler for system calls
* Using value in eax to select a system call
* ebx and edx are paramaters for a system call
*
*/
__attribute__((interrupt)) void interrupt_handler_55(interrupt_frame* frame){
    uint32_t eax;
    uint32_t ebx;
    uint32_t edx;
    asm volatile("mov %%eax, %0\n\t"
                 "mov %%ebx, %1\n\t"
                 "mov %%edx, %2\n\t"
        :"=r"(eax), "=r"(ebx), "=r"(edx)::"eax", "ebx","edx"); 

     switch(eax)
    {
        case 0:
            print_string("do something\n");
            break;

        case 1:
            break;

        case 2:
            break;

        default:
            print_string("Error! unknown system call number");
    }
    //__asm__ __volatile__ ("hlt");
}