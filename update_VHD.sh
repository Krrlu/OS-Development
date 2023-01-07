#!/bin/bash

dd if=boot.bin of=$1 bs=512 count=1 conv=notrunc #update bootloader
dd if=kernel.bin of=$1 bs=512 count=3 seek=1 conv=notrunc #update kernel
