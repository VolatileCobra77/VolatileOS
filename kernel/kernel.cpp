// #include "./modules/rendering/vga_text.h"

// static inline void outb(unsigned short port, unsigned char val) {
//     asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
// }



// void serial_init() {
//     outb(0x3F8 + 1, 0x00);
//     outb(0x3F8 + 3, 0x80);
//     outb(0x3F8 + 0, 0x03);
//     outb(0x3F8 + 1, 0x00);
//     outb(0x3F8 + 3, 0x03);
//     outb(0x3F8 + 2, 0xC7);
//     outb(0x3F8 + 4, 0x0B);
// }

// void serial_write(char c) {
//     outb(0x3F8, c);
// }


// void drawChar(uint32_t* framebuffer, int fbWidth, int x, int y, char c, uint32_t color) {
//     const uint8_t* glyph = font8x8_basic[(uint8_t)c]; // 8 bytes per char
//     for (int row = 0; row < 8; row++) {
//         for (int col = 0; col < 8; col++) {
//             if (glyph[row] & (1 << col)) {
//                 framebuffer[(y + row) * fbWidth + (x + col)] = color;
//             }
//         }
//     }
// }


// extern "C" void kernel_main() {
//     volatile unsigned short* vga = (volatile unsigned short*)TERMINAL_VGA;
    

//     Terminal term = Terminal( 80, 25, TERMINAL_VGA);
//     term.clear();
//     term.printChar('B', TerminalColor(false, Color3B::BLACK, Color4B::WHITE), 1);
    
    

//     volatile unsigned short a = 0b0;
//     a |= (TerminalColor(false, Color3B::BLACK, Color4B::WHITE).getColor() << 0x8);
//     a |= 'A';

//     //KernelPointers::vga[0] = a;
//     vga[0] = a;
//     // for (int i =0; i < 80*25; i+=2){
//     //     vga[i] = ' ';
//     //     vga[i +1] = TerminalColor(false, Color3B::BLACK, Color4B::WHITE).getColor();
//     // }

//     while (true) asm volatile("hlt");
// }

#pragma once
#include <cstdint>
#include "modules/rendering/terminal_graphics.h"
// Minimal Multiboot2 info structure
struct MultibootInfo {
    uint32_t total_size;
    uint32_t reserved;
    // tags follow immediately
} __attribute__((packed));

// Multiboot2 tag header
struct MultibootTag {
    uint32_t type;
    uint32_t size;
} __attribute__((packed));

// Framebuffer tag (type = 8)
struct FramebufferTag {
    uint32_t type;                 // should be 8
    uint32_t size;                 // total size of this tag
    uint64_t framebuffer_addr;     // 64-bit pointer to framebuffer
    uint32_t framebuffer_pitch;    // bytes per line
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;      // bits per pixel
    uint8_t  framebuffer_type;     // 0 = indexed, 1 = RGB, 2 = EGA text
    uint16_t reserved;
    uint32_t red_mask_size;
    uint32_t red_mask_shift;
    uint32_t green_mask_size;
    uint32_t green_mask_shift;
    uint32_t blue_mask_size;
    uint32_t blue_mask_shift;
} __attribute__((packed));
extern "C" void kernel_main(uintptr_t multiboot_ptr) {
    MultibootInfo* mbi = reinterpret_cast<MultibootInfo*>(multiboot_ptr);

    // Scan the tags for the framebuffer tag
    FramebufferTag* fbTag = nullptr;
    uint8_t* tag_ptr = reinterpret_cast<uint8_t*>(mbi) + 8; // skip total_size + reserved
    while (tag_ptr < reinterpret_cast<uint8_t*>(mbi) + mbi->total_size) {
        uint32_t* tag_type = reinterpret_cast<uint32_t*>(tag_ptr);
        uint32_t* tag_size = reinterpret_cast<uint32_t*>(tag_ptr + 4);

        if (*tag_type == 8) { // 8 = framebuffer tag
            fbTag = reinterpret_cast<FramebufferTag*>(tag_ptr);
            break;
        }

        tag_ptr += (*tag_size + 7) & ~7; // 8-byte alignment
    }

    if(!fbTag) return; // no framebuffer found

    // framebuffer pointer
    uint32_t* framebuffer = reinterpret_cast<uint32_t*>(fbTag->framebuffer_addr_low);
    int fb_width = fbTag->width;
    int fb_height = fbTag->height;

    // Now pass this to your TerminalGraphics
    TerminalGraphics term(framebuffer, fb_width, fb_height);
    term.clear(0x000000);
    term.printChar('A', 0xFFFFFF);
    term.printChar('B', 0xFF0000);

    while(1) asm volatile("hlt");
}
