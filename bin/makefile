LD = i686-elf-ld
KERNEL = kernel.o print.o mmu.o io.o isr.o task.o

kernel.elf: $(KERNEL)
	$(LD) -static -T kernel.ld -nostdlib -o kernel.elf $^

boot.elf: boot.o
	$(LD) -static -T linker.ld -nostdlib -o boot.elf boot.o

clean:
	rm *.o *.elf *.bin

%.bin: %.elf
	objcopy -O binary $< $@