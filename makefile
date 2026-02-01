CC = g++
AS = nasm
LD = ld

CFLAGS = -ffreestanding -fno-exceptions -fno-rtti -nostdlib -Wall -Wextra
LDFLAGS = -T kernel/linker.ld

build/kernel.o:
	$(CC) $(CFLAGS) -c kernel/kernel.cpp -o build/kernel.o

build/boot.o:
	$(AS) -f elf64 kernel/boot.asm -o build/boot.o

build/kernel.bin: build/boot.o build/kernel.o
	$(LD) $(LDFLAGS) build/boot.o build/kernel.o -o build/kernel.bin

iso: build/kernel.bin
	mkdir -p iso/boot
	cp build/kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o build/myos.iso iso

clean:
	rm -rf build iso/boot/kernel.bin
