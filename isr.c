#include "print.h"
#include "isr.h"
/*
    This file contain Interrupt Service Routine
*/
void isr0(){
    print_string("divide by zero");
    __asm__ __volatile__ ("hlt");
}