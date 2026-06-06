#include "WS2812.h"
#include "generated/ws2812.pio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"

static PIO  s_pio = pio0;
static uint s_sm  = 0;

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)g << 24) | ((uint32_t)r << 16) | ((uint32_t)b << 8);
}

void WS2812_init(void) {
    uint offset = pio_add_program(s_pio, &ws2812_program);
    ws2812_program_init(s_pio, s_sm, offset, WS2812_PIN, WS2812_FREQ, WS2812_IS_RGBW);
}

void WS2812_fill(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = urgb_u32(r, g, b);
    for (int i = 0; i < WS2812_COUNT; i++) {
        pio_sm_put_blocking(s_pio, s_sm, color);
    }
}

void WS2812_clear(void) {
    WS2812_fill(0, 0, 0);
}
