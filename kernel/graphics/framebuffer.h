#pragma once
#include <stdint.h>

extern volatile uint32_t *fb_ptr ;
extern uint32_t fb_width;
extern uint32_t fb_height ;
extern uint32_t fb_pitch ; // pixels per row

void framebuffer_init(void);
void put_pixel(uint32_t x, uint32_t y, uint32_t color);
void clear_screen(uint32_t color);
void draw_char(uint32_t x, uint32_t y, uint32_t color, char c, const uint8_t font[128][7], int fontsize);
void draw_string(uint32_t x, uint32_t y, uint32_t color, const char *str, const uint8_t font[128][7], int fontsize);
