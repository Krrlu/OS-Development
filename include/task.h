#include <stdint.h>
#include "kernel.h"

extern struct task kernel_task;


// task link is double link list
struct task {
    int id;                 // id of task    
    char padding[4];       // padding
    selector tss_sele;      // tss selector of task
    void* base_addr;        // base adress of program
    struct task* next;             
    struct task* prev;             
}__attribute__((packed)) ;

void create_task(int32_t lba);