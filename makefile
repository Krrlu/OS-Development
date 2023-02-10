CC = i686-elf-gcc
INCLUDE = -I ./include
CFLAGS = -c -g -Os -march=i686 -ffreestanding -Wall -std=gnu17
OBJ = $(patsubst %.c,./bin/%.o, $(wildcard *.c))

all: boot kernel

./bin/%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

boot: ./bin/boot.o
	$(MAKE) -C ./bin boot.bin

interr:
	$(MAKE) -C ./interrupt


kernel: interr $(OBJ)
	$(MAKE) -C ./bin kernel.bin

%.bin: ./bin/%.elf 
	objcopy -O binary $< $@

clean:
	$(MAKE) -C ./bin clean