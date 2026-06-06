#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WS2812_PIN     18
#define WS2812_COUNT   12
#define WS2812_FREQ    800000
#define WS2812_IS_RGBW false

/**
 * Initialisiert den WS2812-PIO-Treiber.
 * Muss auf Core1 aufgerufen werden (oder vor multicore_launch_core1).
 */
void WS2812_init(void);

/**
 * Setzt alle LEDs auf eine Farbe (GRB-Reihenfolge intern).
 * r, g, b: 0–255
 */
void WS2812_fill(uint8_t r, uint8_t g, uint8_t b);

/**
 * Schaltet alle LEDs aus.
 */
void WS2812_clear(void);

#ifdef __cplusplus
}
#endif
