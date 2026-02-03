# nasm -f elf64 kernel/boot.asm -o build/boot.o
# g++ -ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector -nostdlib -Wall -Wextra -O0 -c kernel/kernel.cpp -o build/kernel.o
# ld -T kernel/linker.ld build/kernel.o build/boot.o -o build/kernel.bin
# grub-file --is-x86-multiboot2 build/kernel.bin
# if [ $? == "1" ]
# then
#   echo "ERROR, kernel.bin file not recognized as multiboot2"
#   exit
# fi
# mkdir -p iso/boot/grub
# cp build/kernel.bin iso/boot/kernel.bin
# grub-mkrescue -o build/myos.iso iso

# Download the latest Limine binary release for the 10.x branch.

make

git clone https://codeberg.org/Limine/Limine.git limine --branch=v10.x-binary --depth=1

# Build "limine" utility.
make -C limine

# Create a directory which will be our ISO root.
mkdir -p iso_root

# Copy the relevant files over.
mkdir -p iso_root/boot
cp -v bin/myos iso_root/boot/
mkdir -p iso_root/boot/limine
cp -v limine/limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin \
      limine/limine-uefi-cd.bin iso_root/boot/limine/

# Create the EFI boot tree and copy Limine's EFI executables over.
mkdir -p iso_root/EFI/BOOT
cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

# Create the bootable ISO.
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o image.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
./limine/limine bios-install image.iso