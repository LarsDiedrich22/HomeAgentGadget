#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>

extern "C" {
#include "DEV_Config.h"
#include "LCD_1in28.h"
#include "CST816S.h"
#include "DisplayInit.h"
#include "SerialProtocol.h"
#include "AgentUI.h"
#include "WS2812.h"
}

#include "lvgl.h"

/* ---- WS2812-Farben für Gate-Zustände (läuft auf Core1) ---- */
static void ws2812_for_msg(const sp_message_t *msg) {
    switch (msg->type) {
        case SP_MSG_START:       WS2812_clear();             break;
        case SP_MSG_APPROACHING: WS2812_fill(80, 50,  0);   break;  /* Gelb  */
        case SP_MSG_CONSENT_NEEDED: WS2812_fill(30,  0, 80); break;  /* Lila  */
        case SP_MSG_ALLOWED:     WS2812_fill( 0, 60,  0);   break;  /* Grün  */
        case SP_MSG_DENIED:      WS2812_clear();             break;
        case SP_MSG_ENTER:
            switch (msg->color) {
                case SP_COLOR_GREEN:  WS2812_fill( 0, 80,  0); break;
                case SP_COLOR_YELLOW: WS2812_fill(80, 50,  0); break;
                case SP_COLOR_RED:    WS2812_fill(80,  0,  0); break;
                default:              WS2812_fill(30, 30, 30); break;
            }
            break;
        case SP_MSG_BETRAYAL:    WS2812_fill(120, 0, 0);    break;  /* Rot   */
        case SP_MSG_DONE:
            switch (msg->outcome) {
                case SP_OUTCOME_SUCCESS:  WS2812_fill( 0, 40,  0);  break;
                case SP_OUTCOME_BETRAYAL: WS2812_fill(80,  0,  0);  break;
                case SP_OUTCOME_BLOCKED:  WS2812_fill(20, 20, 20);  break;
                default: WS2812_clear(); break;
            }
            break;
        default: break;
    }
}

/* ---- Core1: WS2812 über multicore FIFO ---- */
/*
 * Packing:  [23:16] = sp_msg_type_t  |  [15:8] = sp_color_t  |  [7:0] = sp_outcome_t
 */
void core1_entry() {
    WS2812_init();
    WS2812_clear();

    for (;;) {
        uint32_t val = multicore_fifo_pop_blocking();
        sp_message_t msg = {};
        msg.type    = (sp_msg_type_t)((val >> 16) & 0xFF);
        msg.color   = (sp_color_t)  ((val >>  8) & 0xFF);
        msg.outcome = (sp_outcome_t)( val         & 0xFF);
        ws2812_for_msg(&msg);
    }
}

static void push_led_state(const sp_message_t *msg) {
    uint32_t val = ((uint32_t)msg->type    << 16)
                 | ((uint32_t)msg->color   <<  8)
                 |  (uint32_t)msg->outcome;
    multicore_fifo_push_blocking(val);
}

/* ---- Consent-Callback (wird aus AgentUI aufgerufen) ---- */
static void on_consent(bool allow) {
    SerialProtocol_send_consent(allow);
}

/* ---- main ---- */
int main(void) {
    stdio_init_all();
    sleep_ms(500);   /* USB-Serial bereit */

    if (DEV_Module_Init() != 0) {
        for (;;) { sleep_ms(500); }
    }

    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    DEV_SET_PWM(100);
    CST816S_init(CST816S_Point_Mode);

    DisplayInit();
    AgentUI_init(on_consent);

    multicore_reset_core1();
    multicore_launch_core1(core1_entry);

    sp_message_t msg;
    for (;;) {
        lv_task_handler();

        if (SerialProtocol_poll(&msg)) {
            AgentUI_handle(&msg);
            push_led_state(&msg);
        }

        DEV_Delay_ms(5);
    }

    return 0;
}
