#!/bin/bash

set -e

aarch64-elf-gcc -c boot.S            -o boot.o   -nostdlib -ffreestanding
aarch64-elf-gcc -c fb.c              -o fb.o     -nostdlib -ffreestanding
aarch64-elf-gcc -c font8x8_basic.c   -o font.o   -nostdlib -ffreestanding
aarch64-elf-gcc -c console.c         -o console.o -nostdlib -ffreestanding
aarch64-elf-gcc -c kernel.c          -o kernel.o -nostdlib -ffreestanding
aarch64-elf-gcc -c string.c          -o string.o -nostdlib -ffreestanding
aarch64-elf-gcc -c fs.c              -o fs.o     -nostdlib -ffreestanding

aarch64-elf-ld -T linker.ld -o kernel.elf \
    boot.o fb.o font.o console.o kernel.o string.o fs.o

aarch64-elf-objcopy -O binary kernel.elf kernel8.img
