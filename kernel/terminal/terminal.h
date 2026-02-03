#include <stdint.h>
typedef struct Terminal {
    // Cursor state
    uint32_t x;
    uint32_t y;


    // Dimensions per character
    uint32_t line_height;
    uint32_t char_width;

    // Function pointers (methods)

    void (*scroll_up)(struct Terminal*, uint32_t pixels);
    void (*putchar)(struct Terminal*, char c, const uint8_t font[128][7], int fontsize, uint32_t color);
    void (*write)(struct Terminal*, const char* str, const uint8_t font[128][7], int fontsize, uint32_t color);

} Terminal;
static Terminal term;

Terminal terminal_init(int fontsize);