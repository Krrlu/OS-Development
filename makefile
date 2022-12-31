CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -std=gnu17

all: boot.bin

boot.o: boot.c
	$(CC) $(CFLAGS) -o boot.o boot.c

boot.elf: boot.o
	$(LD) -static -T linker.ld -nostdlib -o boot.elf boot.o

boot.bin: boot.elf
	objcopy -O binary boot.elf boot.bin

clean:
	rm *.o
	rm *.elf
	rm *.bin
