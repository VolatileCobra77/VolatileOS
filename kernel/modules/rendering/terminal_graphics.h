// terminal_graphics.h
#pragma once
#include <cstdint>
#include "font.h"

class TerminalGraphics {
public:
    uint32_t* framebuffer;
    int width;
    int height;
    int cursorX;
    int cursorY;
    int charWidth = 8;
    int charHeight = 8;

    TerminalGraphics(uint32_t* fb, int w, int h)
        : framebuffer(fb), width(w), height(h), cursorX(0), cursorY(0) {}

    void putPixel(int x, int y, uint32_t color) {
        if(x < 0 || x >= width || y < 0 || y >= height) return;
        framebuffer[y * width + x] = color;
    }

    void drawChar(char c, int x, int y, uint32_t color) {
        const uint8_t* glyph = font8x8_basic[(uint8_t)c];
        for(int row = 0; row < charHeight; row++) {
            for(int col = 0; col < charWidth; col++) {
                if(glyph[row] & (1 << (7-col))) {  // MSB first
                    putPixel(x + col, y + row, color);
                }
            }
        }
    }

    void printChar(char c, uint32_t color) {
        drawChar(c, cursorX * charWidth, cursorY * charHeight, color);
        cursorX++;
        if(cursorX * charWidth >= width) {
            cursorX = 0;
            cursorY++;
        }
        if(cursorY * charHeight >= height) cursorY = 0; // simple wrap
    }

    void clear(uint32_t color = 0x000000) {
        for(int y = 0; y < height; y++)
            for(int x = 0; x < width; x++)
                framebuffer[y * width + x] = color;
        cursorX = 0;
        cursorY = 0;
    }
};
