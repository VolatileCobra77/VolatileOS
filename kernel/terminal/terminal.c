#include <stdint.h>;
#include "../graphics/framebuffer.h";
#include "./terminal.h";


uint32_t cursorY = 0;
uint32_t cursorX = 0;
int fontsize = 3;

void scroll_screen(uint32_t pixels){
    if (!fb_ptr) return;

    for (uint32_t y = 0; y < fb_height - pixels; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            fb_ptr[y * fb_pitch + x] = fb_ptr[(y + pixels) * fb_pitch + x];
        }
    }


}