#include "DisplayInit.h"
#include "DEV_Config.h"
#include "LCD_1in28.h"
#include "CST816S.h"
#include "QMI8658.h"

#include "lvgl.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "pico/time.h"

/* ---------- Display resolution ---------- */
#define DISP_HOR_RES  240
#define DISP_VER_RES  240

/* ---------- LVGL internal buffers ---------- */
static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf0[DISP_HOR_RES * DISP_VER_RES / 2];
static lv_color_t buf1[DISP_HOR_RES * DISP_VER_RES / 2];
static lv_disp_drv_t   disp_drv;

/* ---------- Touch input ---------- */
static lv_indev_drv_t   indev_ts;
static uint16_t ts_x, ts_y;
static lv_indev_state_t ts_act = LV_INDEV_STATE_RELEASED;

/* ---------- DMA (definiert in lib/Config/DEV_Config.c, extern via DEV_Config.h) ---------- */
/* dma_tx und c werden über DEV_Config.h eingebunden */

/* ---------- Repeating timer ---------- */
static struct repeating_timer lvgl_timer;

/* ---------------------------------------------------------- */
static void disp_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    LCD_1IN28_SetWindows(area->x1, area->y1, area->x2, area->y2);
    dma_channel_configure(
        dma_tx, &c,
        &spi_get_hw(LCD_SPI_PORT)->dr,
        color_p,
        ((area->x2 + 1 - area->x1) * (area->y2 + 1 - area->y1)) * 2,
        true
    );
}

static void touch_callback(uint gpio, uint32_t events)
{
    if (gpio == Touch_INT_PIN) {
        CST816S_Get_Point();
        ts_x   = Touch_CTS816.x_point;
        ts_y   = Touch_CTS816.y_point;
        ts_act = LV_INDEV_STATE_PRESSED;
    }
}

static void ts_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    data->point.x = ts_x;
    data->point.y = ts_y;
    data->state   = ts_act;
    ts_act        = LV_INDEV_STATE_RELEASED;
}

static void dma_handler(void)
{
    if (dma_channel_get_irq0_status(dma_tx)) {
        dma_channel_acknowledge_irq0(dma_tx);
        lv_disp_flush_ready(&disp_drv);
    }
}

static bool repeating_lvgl_timer_callback(struct repeating_timer *t)
{
    lv_tick_inc(5);
    return true;
}

/* ---------------------------------------------------------- */
void DisplayInit(void)
{
    /* 1. LVGL core */
    lv_init();

    /* 2. Display draw buffer */
    lv_disp_draw_buf_init(&disp_buf, buf0, buf1, DISP_HOR_RES * DISP_VER_RES / 2);

    /* 3. Display driver */
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res  = DISP_HOR_RES;
    disp_drv.ver_res  = DISP_VER_RES;
    lv_disp_drv_register(&disp_drv);

    /* 4. Touch screen input device */
    lv_indev_drv_init(&indev_ts);
    indev_ts.type    = LV_INDEV_TYPE_POINTER;
    indev_ts.read_cb = ts_read_cb;
    lv_indev_drv_register(&indev_ts);
    DEV_IRQ_SET(Touch_INT_PIN, GPIO_IRQ_EDGE_RISE, &touch_callback);

    /* 5. DMA interrupt */
    dma_channel_set_irq0_enabled(dma_tx, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    /* 6. LVGL tick timer (5 ms) */
    add_repeating_timer_ms(5, repeating_lvgl_timer_callback, NULL, &lvgl_timer);
}
