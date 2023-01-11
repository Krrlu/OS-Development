#include <sys/io.h>
#include <stdint.h>
#include "io.h"
/**
 * Read  from disk
 *
 * @param lba LBA number
 * @param buffer Adress where data will be stored
 * @param sectors Number of sector to read
 */

void read_disk(uint32_t lba, uint16_t* buffer, unsigned char sectors) {

    outb(sectors,0x1f2); // number of sector to read

    outb(lba & 0xff,0x1f3); //send bit 0 - 7 of LBA
    outb((lba >> 8) & 0xff,0x1f4); //send bit 8 - 15 of LBA
    outb((lba >> 16) & 0xff,0x1f5); //send bit 16 - 23 of LBA
    outb((lba >> 24) | 0xe0,0x1f6); //send bit 24 - 27 of LBA

    outb(0x20,0x1f7); //read

    unsigned char status = inb(0x1f7);

    while(!(status & 8)){
        status = inb(0x1f7);
    }
    for(int i = 0; i < (sectors * 256); i++){
        buffer[i] = inw(0x1f0);
    }
}