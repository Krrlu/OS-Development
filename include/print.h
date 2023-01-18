#include <stdint.h>
void print_char(char c);
void print_string(char* s);
void print_hex(uint32_t n);

#define print_new_line() print_char('\n')