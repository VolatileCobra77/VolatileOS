#include <stdint.h>
#include <stddef.h>
#include "../limine.h"
#include "framebuffer.h"  // define your public API

// Global framebuffer info

volatile uint32_t *fb_ptr;
uint32_t fb_width =0;
uint32_t fb_height =0;
uint32_t fb_pitch =0; // pixels per row

// Limine framebuffer request
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// Initialize framebuffer
void framebuffer_init(void) {
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {
        // no framebuffer, hang
        for (;;) asm("hlt");
    }

    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
    fb_ptr = fb->address;
    fb_width = fb->width;
    fb_height = fb->height;
    fb_pitch = fb->pitch / 4; // bytes -> pixels
}

// Put a pixel at (x, y)
void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb_ptr) return;
    if (x >= fb_width || y >= fb_height) return;
    fb_ptr[y * fb_pitch + x] = color;
}

// Fill the screen with a color
void clear_screen(uint32_t color) {
    if (!fb_ptr) return;
    for (uint32_t y = 0; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            put_pixel(x, y, color);
        }
    }
}

// Draw a character using a 5x7 bitmap font
void draw_char(uint32_t x, uint32_t y, uint32_t color, char c, const uint8_t font[128][7], int fontsize) {
    if (!fb_ptr || fontsize < 1) return;

    // Original glyph size
    const int glyph_width  = 5;
    const int glyph_height = 7;

    for (int row = 0; row < glyph_height * fontsize; row++) {
        for (int col = 0; col < glyph_width * fontsize; col++) {
            // Map the scaled row/col to the original glyph
            int glyph_row = row / fontsize;
            int glyph_col = col / fontsize;

            // Check if the original glyph pixel is set
            if ((font[(uint8_t)c][glyph_row] >> (glyph_width - 1 - glyph_col)) & 1) {
                put_pixel(x + col, y + row, color);
            }
        }
    }
}


// Draw a string horizontally
void draw_string(uint32_t x, uint32_t y, uint32_t color, const char *str, const uint8_t font[128][7], int fontsize) {
    while (*str) {
        draw_char(x, y, color, *str, font, fontsize);
        x += (6*fontsize); // 5 pixels width + 1 pixel spacing
        str++;
    }
}
