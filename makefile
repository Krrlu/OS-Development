CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -c -g -Os -march=i686 -ffreestanding -Wall -I ./include -std=gnu17 -mgeneral-regs-only

all: boot.bin kernel.bin

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

boot.elf: boot.o
	$(LD) -static -T linker.ld -nostdlib -o boot.elf boot.o

kernel.elf: kernel.o print.o mmu.o io.o isr.o task.o
	$(LD) -static -T kernel.ld -nostdlib -o kernel.elf $^

%.bin: %.elf 
	objcopy -O binary $< $@

clean:
	rm *.o *.elf *.bin