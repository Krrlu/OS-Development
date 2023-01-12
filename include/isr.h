#include <stdint.h>
typedef struct
{
    uint32_t eip;
    uint16_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint16_t ss;
}__attribute__((packed)) interrupt_frame;

void interrupt_handler_0(interrupt_frame* frame);
void interrupt_handler_3(interrupt_frame* frame);
void interrupt_handler_6(interrupt_frame* frame);
void interrupt_handler_55(interrupt_frame* frame);