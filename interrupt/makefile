CC = i686-elf-gcc
CFLAGS = -c -g -Os -march=i686 -ffreestanding -Wall -std=gnu17
INCLUDE = -I ../include

OBJ = $(patsubst %.c,../bin/%.o, $(wildcard *.c))

all: $(OBJ)

../bin/%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE) -mgeneral-regs-only