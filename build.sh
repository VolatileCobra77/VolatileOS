nasm -f elf64 kernel/boot.asm -o build/boot.o
g++ -ffreestanding -fno-exceptions -fno-rtti -nostdlib -Wall -Wextra -c kernel/kernel.cpp -o build/kernel.o
ld -T kernel/linker.ld build/kernel.o build/boot.o -o build/kernel.bin
grub-file --is-x86-multiboot2 build/kernel.bin
if [ $? == "1" ]
then
  echo "ERROR, kernel.bin file not recognized as multiboot2"
  exit
fi
mkdir -p iso/boot/grub
cp build/kernel.bin iso/boot/kernel.bin
grub-mkrescue -o build/myos.iso iso