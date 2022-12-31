#!/bin/bash

dd if=boot.bin of=/media/sf_asm/learn.vhd bs=512 count=1 conv=notrunc #update bootloader(ignore the file location)
