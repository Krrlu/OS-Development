#define VGA_BUFFER (char*)0xb8000
#define VGA_WIDTH 80 
#define  TEXT_ATTRIBUTE 7

void print_char(char c);
void print_string(char* s);