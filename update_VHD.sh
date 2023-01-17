#!/bin/bash
size=$(wc -c < kernel.bin)
sectors=$((size/512+1))

dd if=boot.bin of=$1 bs=512 count=1 conv=notrunc #update bootloader
dd if=kernel.bin of=$1 bs=512 count=$sectors seek=1 conv=notrunc #update kernel
