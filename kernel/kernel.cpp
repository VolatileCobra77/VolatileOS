// kernel/kernel.cpp

static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void serial_init() {
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

void serial_write(char c) {
    outb(0x3F8, c);
}

extern "C" void kernel_main() {
    serial_init();
    volatile unsigned short* vga = (volatile unsigned short*)0xB8000;

    // Clear the screen (80x25 characters)
    for (int i = 0; i < 80*25; i++) {
        vga[i] = ' ' | (0x0F << 8); // space character, white on black
    }
    const char* msg = "Hello from serial!\n";
    for (int i = 0; msg[i]; i++) {
        serial_write(msg[i]);
    }

    while (true) asm volatile("hlt");
}