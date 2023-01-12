#include "print.h"
#include "isr.h"
/*
    This file contain Interrupt Service Routine
*/
void isr0(){
    print_string("error, divide by zero");
    __asm__ __volatile__ ("hlt");
}

void isr6(){
    print_string("error, Invalid Opcode");
    __asm__ __volatile__ ("hlt");
}

void isr13(){
    print_string("error, general protection");
    __asm__ __volatile__ ("hlt");
}