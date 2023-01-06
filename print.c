#include <sys/io.h>
#include <stdint.h>
#include "print.h"

uint16_t get_cursor_position(){
    uint16_t pos = 0;

    outb(0x0F, 0x3D4);
    pos |= inw(0x3D5);

    outb(0x0E, 0x3D4);
    pos |= ((uint16_t)inw(0x3D5)) << 8;

    return pos;
}

void set_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x0F, 0x3D4);
	outb((uint8_t)(pos & 0xFF),0x3D5);
	outb(0x0E,0x3D4);
	outb((uint8_t)((pos >> 8) & 0xFF),0x3D5);
}


void print_char(char c){
    
    char* buffer = VGA_BUFFER;
    uint32_t pos = ((uint32_t)(get_cursor_position())) << 1; //offset in VGA buffer
    *(buffer + pos) = c; //character
    *(buffer + pos+1) = 7; //color
}