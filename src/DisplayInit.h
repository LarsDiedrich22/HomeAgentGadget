#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes LVGL, the LCD display, touch screen, DMA, and repeating timer.
 * Call once before using any LVGL functions.
 */
void DisplayInit(void);

#ifdef __cplusplus
}
#endif
