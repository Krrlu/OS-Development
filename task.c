#include "task.h"
#include "io.h"
#include "mmu.h"
#include "print.h"

#define STACK_SIZE 512

struct header {
    uint16_t header_size;   // size of header          
    uint16_t program_size;  // size of program, include header 
    uint16_t data_end;      // offset from start of header to end section
    uint16_t entry_point;   // offset from start of header to entry of program
}__attribute__((packed));


/**
 * Create a new task
 *
 * @param lba The lba of program to load in disk
 * 
 */

void create_task(int32_t lba){
    void* base_addr = malloc(512);
    tss* task_tss = malloc(sizeof(tss));
    struct task* user_task = malloc(sizeof(struct task));
    struct header h;

    
    read_disk(lba,base_addr,1);
    h = *(struct header*) base_addr;

    // print_hex(h.header_size);
    // print_hex(h.program_size);
    // print_hex(h.data_offset);
    // print_hex(h.entry_point);
    void* stack0 = malloc(512); // size of stack is 512 b
    void* stack3 = malloc(512);
    SegmentDescriptor ss_0  = construct_seg_descriptor((uint32_t)malloc(512),512,ATTRIBUTE_DATA_DESCRIPTOR_RING0);
    SegmentDescriptor ss_3  = construct_seg_descriptor((uint32_t)malloc(512),512,ATTRIBUTE_DATA_DESCRIPTOR_RING3);

    SegmentDescriptor ds = construct_seg_descriptor((uint32_t)base_addr,h.data_end - h.header_size,ATTRIBUTE_DATA_DESCRIPTOR_RING3);
    task_tss->ss_0 = install_descriptor(&ss_0); // this kernel only use two privilege levels, 0 and 3
    task_tss->ss = install_descriptor(&ss_3) | 0b11; // set rpl = 3 by OR 0b11
    task_tss->ds = install_descriptor(&ds) | 0b11;
    task_tss->es = task_tss->ds;
    task_tss->gs = task_tss->ds;
    task_tss->fs = task_tss->ds;
    task_tss->cs = USER_CODE_SEL;
    task_tss->esp = (uint32_t)((char*) stack3 + STACK_SIZE) | 0b11;
    task_tss->esp_0 = (uint32_t)(char*) stack0 + STACK_SIZE;
    task_tss->ldt = 0;
    task_tss->previous_task_link = 0;
    task_tss->T = 0;
    task_tss->eip = (uint32_t)((char*)base_addr + h.entry_point);

    //save cr3 to tss
    asm ("mov %%cr3, %%eax\n\t"
         "mov %%eax, %0\n\t"
        :"=r"(task_tss->cr3)
        ::"%eax");

    //save eflag to tss
    asm ("pushfl\n\t"
         "popl %0\n\t"
        ::"m"(task_tss->eflags):);
    
    SegmentDescriptor temp_tss = construct_seg_descriptor((uint32_t)task_tss,SIZE_TSS,ATTRIBUTE_TSS_DESCRIPTOR);
    user_task->tss_sele = install_descriptor(&temp_tss);

    // append user_task to the task linklist
    struct task* p = &kernel_task;
    while(p->next != NULL) p = p->next;
    p->next = user_task;
    user_task->prev = p;
    user_task->id = p->id + 1;
    user_task->busy = 0;
}

/**
 *  task_switch() first find a avaiable task and the busy task in the task link,
 *  then set the busy task to avaiable and switch to the new task
 * 
 */

void task_switch(){
    struct task* busy_task = NULL;
    struct task* avaiable_task = NULL;
    struct task* p = &kernel_task;

    // find tasks in task link
    while(p != NULL && (busy_task == NULL || avaiable_task == NULL) ){
        if(busy_task == NULL && p->busy == 1) busy_task = p;
        if(avaiable_task == NULL && p->busy == 0) avaiable_task = p;
        p = p->next;
    }

    if(busy_task == NULL) panic("Cannot find a busy task");

    //set busy task to avaiable and avaiable task to busy
    busy_task->busy = 0;
    avaiable_task->busy = 1;

    // task switching, jump to the memory contain TSS selector
    // ljmpl = jmp far [memory] 
    asm ("ljmpl %0"::"m"(avaiable_task->padding):); 

}