#include "terminal.h"
#include "../graphics/framebuffer.h"
#include "../graphics/font.h"

void scroll_up(Terminal* self, uint32_t pixels){
    if (!fb_ptr) return;


    //move whole framebuffer up X pixels
    for (uint32_t y = 0; y < fb_height - pixels; y++){
        for (uint32_t x = 0; x < fb_width; x++) {
            fb_ptr[y * fb_pitch + x] = fb_ptr[(y + pixels) * fb_pitch + x];
        }
    }

    //clear bottom row
    for (uint32_t y = fb_height - pixels; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            fb_ptr[y * fb_pitch + x] = COLOR_BLACK;
        }
    }

    //reset bounds
    if (self->y >= pixels)
        self->y -= pixels;
    else
        self->y = 0;
}

static void putchar(Terminal* self, char c, const uint8_t font[128][7], int fontsize, uint32_t color){
    if (c == '\n'){
        self->x = 0;
        self->y += self->line_height;

        if ((self->y + self->line_height) > fb_height) {
            self->scroll_up(self, self->line_height);
        }
        return;
    }

    draw_char(self->x, self->y, color, c, font, fontsize);
    self->x += self->char_width;
    if (self->x + self->char_width > fb_width) {
        self->x = 0;
        self->y += self->line_height;
        if (self->y + self->line_height > fb_height) {
            self->scroll_up(self, self->line_height);
        }
    }
}

static void write(Terminal *self, const char *str, const uint8_t font[128][7], int fontsize, uint32_t color) {
    while (*str) {
        self->putchar(self, *str, font, fontsize, color);
        str++;
    }
}
Terminal terminal_init(int fontsize) {
    term.x = 0;
    term.y = 0;
    term.line_height = FONT_HEIGHT * fontsize;
    term.char_width  = FONT_WIDTH * fontsize + 1;

    term.scroll_up = scroll_up;
    term.putchar   = putchar;
    term.write     = write;
    return term;
}
