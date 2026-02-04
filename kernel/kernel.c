#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "./graphics/framebuffer.h"
#include "./graphics/font.h"
#include "./drivers/keyboard.h"
#include "drivers/timers.h"
#include "terminal/terminal.h"
#include "./drivers/idt.h"

// // Set the base revision to 4, this is recommended as this is the latest
// // base revision described by the Limine boot protocol specification.
// // See specification for further info.

// __attribute__((used, section(".limine_requests")))
// static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

// // The Limine requests can be placed anywhere, but it is important that
// // the compiler does not optimise them away, so, usually, they should
// // be made volatile or equivalent, _and_ they should be accessed at least
// // once or marked as used with the "used" attribute as done here.


// // Finally, define the start and end markers for the Limine requests.
// // These can also be moved anywhere, to any .c file, as seen fit.

// __attribute__((used, section(".limine_requests_start")))
// static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

// __attribute__((used, section(".limine_requests_end")))
// static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    uint8_t *restrict pdest = (uint8_t *restrict)dest;
    const uint8_t *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

extern void irq0_stub(void);

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kernel_main(void) {
    //idt_init();
    //idt_set_gate(32, pit_irq_handler);

    //timer_init();
    asm volatile ("sti");
    framebuffer_init();
    clear_screen(COLOR_BLACK);
    draw_string(10, 10, COLOR_WHITE, "Welcome to VolatileOS", font_monospace, 3);
    //sleep_ms(1000);
    scroll_screen(20);

    // We're done, just hang...
    asm volatile ("cli");
    hcf();
}