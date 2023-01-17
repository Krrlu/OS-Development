#include <sys/io.h>
#include "print.h"
#define VGA_BUFFER (char*)0xb8000
#define VGA_WIDTH 80 
#define CHARACTER_ATTRIBUTE 7

static const char* lookup_hex = "0123456789abcdef";


uint16_t static inline get_cursor_position(){
    uint16_t pos = 0;

    outb(0x0F, 0x3D4);
    pos |= inw(0x3D5);

    outb(0x0E, 0x3D4);
    pos |= ((uint16_t)inw(0x3D5)) << 8;

    return pos;
}

void static inline set_cursor(uint16_t pos)
{
 
	outb(0x0F, 0x3D4);
	outb((uint8_t)(pos & 0xFF),0x3D5);
	outb(0x0E,0x3D4);
	outb((uint8_t)((pos >> 8) & 0xFF),0x3D5);
}

void print_string(char* s){
    char* buffer = VGA_BUFFER;
    uint32_t pos = (uint32_t)(get_cursor_position()); //offset in VGA buffer
    while(*s){
        if(*s == '\n'){
        pos = ((pos / VGA_WIDTH) + 1) * VGA_WIDTH;
    }else{
        *(buffer + (pos << 1)) = *s; //character
        *(buffer + (pos << 1) + 1) = CHARACTER_ATTRIBUTE;
    pos++;
    }
    s++;
    }
    set_cursor(pos);
}

void print_char(char c){
    
    char* buffer = VGA_BUFFER;
    uint32_t pos = (uint32_t)(get_cursor_position()); //offset in VGA buffer
    if(c == '\n'){
        pos = ((pos / VGA_WIDTH) + 1) * VGA_WIDTH;
    }else{
        *(buffer + (pos << 1)) = c; //character
        *(buffer + (pos << 1) + 1) = CHARACTER_ATTRIBUTE;
    pos++;
    }
    set_cursor(pos);
}

/**
 * Print a number in hex decimal
 */

void print_hex(uint32_t n){
    char nums[9];
    for(int i = 7; i >=0; i--){
        nums[i] = lookup_hex[n & 0xf];
        n >>= 4;
    }
    nums[8] = '\0'; // end of string
    print_string("0x");
    print_string(nums);
    print_char('\n');
}
