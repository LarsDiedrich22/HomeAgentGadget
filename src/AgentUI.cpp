/*
 * AgentUI.cpp  –  LVGL-UI für Agent-Trust auf dem 240×240 runden Display
 *
 * Layout (runder 240×240 Bildschirm):
 *   - Oben:       Titelzeile + Modus-Anzeige (kleine Chips)
 *   - Mitte:      3 Gate-Cards vertikal gestapelt (Kalender / Kontakte / Mail)
 *                 mit Statusdot + Label + Statustext
 *   - Unten:      Outcome-Label
 *   - Overlay:    Consent-Dialog (zentriert, halbdurchsichtiger Hintergrund)
 */

#include "AgentUI.h"
#include "lvgl.h"
#include <string.h>

/* ---- Farben (RGB565 als LVGL lv_color_hex) ---- */
#define C_BG          lv_color_hex(0x1b1b19)
#define C_CARD        lv_color_hex(0x242422)
#define C_BORDER_IDLE lv_color_hex(0x3a3a38)
#define C_TEXT        lv_color_hex(0xeeede8)
#define C_SUBTEXT     lv_color_hex(0x9c9a92)

#define C_GREEN       lv_color_hex(0x639922)
#define C_YELLOW      lv_color_hex(0xEF9F27)
#define C_RED         lv_color_hex(0xE24B4A)
#define C_PURPLE      lv_color_hex(0x7f77dd)
#define C_PURPLE_BG   lv_color_hex(0x3c3489)

/* ---- Gate-Definitionen ---- */
static const char *ROOM_LABELS[3]  = {"Kalender lesen", "Kontakte lesen", "Mail senden"};
static const char *ROOM_IDS[3]     = {"kalender", "kontakte", "mail"};
static const char *ROOM_DESC[3]    = {"Verfügbarkeit prüfen", "Absender identifizieren", "Rotes Gate · irreversibel"};

/* ---- Widget-Handles ---- */
static lv_obj_t *scr;

/* Gate-Cards */
static lv_obj_t *card[3];
static lv_obj_t *dot[3];
static lv_obj_t *status_lbl[3];

/* Modus-Chips */
static lv_obj_t *mode_lbl;

/* Outcome */
static lv_obj_t *outcome_lbl;

/* Consent-Overlay */
static lv_obj_t *consent_overlay;
static lv_obj_t *consent_room_lbl;
static lv_obj_t *consent_allow_btn;
static lv_obj_t *consent_deny_btn;

/* Callback */
static void (*s_consent_cb)(bool allow) = NULL;

/* ---- Hilfsfunktionen ---- */

static int room_index(const char *room_id) {
    for (int i = 0; i < 3; i++) {
        if (strcmp(ROOM_IDS[i], room_id) == 0) return i;
    }
    return -1;
}

static lv_color_t sp_color_to_lv(sp_color_t c) {
    switch (c) {
        case SP_COLOR_GREEN:  return C_GREEN;
        case SP_COLOR_YELLOW: return C_YELLOW;
        case SP_COLOR_RED:    return C_RED;
        default:              return lv_color_hex(0x444444);
    }
}

static void set_dot_color(int i, lv_color_t color) {
    lv_obj_set_style_bg_color(dot[i], color, 0);
}

static void set_card_border(int i, lv_color_t color) {
    lv_obj_set_style_border_color(card[i], color, 0);
}

static void reset_card(int i) {
    lv_obj_set_style_border_color(card[i], C_BORDER_IDLE, 0);
    lv_obj_set_style_bg_color(dot[i], lv_color_hex(0x444444), 0);
    lv_label_set_text(status_lbl[i], "");
    lv_obj_set_style_opa(card[i], LV_OPA_COVER, 0);
}

/* ---- Consent-Buttons ---- */
static void on_allow(lv_event_t *e) {
    lv_obj_add_flag(consent_overlay, LV_OBJ_FLAG_HIDDEN);
    if (s_consent_cb) s_consent_cb(true);
}
static void on_deny(lv_event_t *e) {
    lv_obj_add_flag(consent_overlay, LV_OBJ_FLAG_HIDDEN);
    if (s_consent_cb) s_consent_cb(false);
}

/* ---- UI aufbauen ---- */

void AgentUI_init(void (*consent_cb)(bool allow)) {
    s_consent_cb = consent_cb;

    scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, C_BG, 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /* Titel */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Agent Trust");
    lv_obj_set_style_text_color(title, C_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Modus-Label */
    mode_lbl = lv_label_create(scr);
    lv_label_set_text(mode_lbl, "–");
    lv_obj_set_style_text_color(mode_lbl, C_SUBTEXT, 0);
    lv_obj_set_style_text_font(mode_lbl, &lv_font_montserrat_10, 0);
    lv_obj_align(mode_lbl, LV_ALIGN_TOP_MID, 0, 26);

    /* Gate-Cards: vertikal, je 44px hoch, 190px breit */
    int card_w = 190, card_h = 44;
    int start_y = 42;
    int gap     = 6;

    for (int i = 0; i < 3; i++) {
        card[i] = lv_obj_create(scr);
        lv_obj_set_size(card[i], card_w, card_h);
        lv_obj_align(card[i], LV_ALIGN_TOP_MID, 0, start_y + i * (card_h + gap));
        lv_obj_set_style_bg_color(card[i], C_CARD, 0);
        lv_obj_set_style_border_color(card[i], C_BORDER_IDLE, 0);
        lv_obj_set_style_border_width(card[i], 1, 0);
        lv_obj_set_style_radius(card[i], 8, 0);
        lv_obj_set_style_pad_all(card[i], 8, 0);
        lv_obj_clear_flag(card[i], LV_OBJ_FLAG_SCROLLABLE);

        /* Dot */
        dot[i] = lv_obj_create(card[i]);
        lv_obj_set_size(dot[i], 8, 8);
        lv_obj_set_style_radius(dot[i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(dot[i], lv_color_hex(0x444444), 0);
        lv_obj_set_style_border_width(dot[i], 0, 0);
        lv_obj_align(dot[i], LV_ALIGN_LEFT_MID, 0, 0);

        /* Gate-Label */
        lv_obj_t *lbl = lv_label_create(card[i]);
        lv_label_set_text(lbl, ROOM_LABELS[i]);
        lv_obj_set_style_text_color(lbl, C_TEXT, 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_12, 0);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 14, -7);

        /* Beschreibung */
        lv_obj_t *desc = lv_label_create(card[i]);
        lv_label_set_text(desc, ROOM_DESC[i]);
        lv_obj_set_style_text_color(desc, C_SUBTEXT, 0);
        lv_obj_set_style_text_font(desc, &lv_font_montserrat_10, 0);
        lv_obj_align(desc, LV_ALIGN_LEFT_MID, 14, 7);

        /* Status */
        status_lbl[i] = lv_label_create(card[i]);
        lv_label_set_text(status_lbl[i], "");
        lv_obj_set_style_text_color(status_lbl[i], C_SUBTEXT, 0);
        lv_obj_set_style_text_font(status_lbl[i], &lv_font_montserrat_10, 0);
        lv_obj_align(status_lbl[i], LV_ALIGN_RIGHT_MID, -2, 0);
    }

    /* Outcome-Label */
    outcome_lbl = lv_label_create(scr);
    lv_label_set_text(outcome_lbl, "");
    lv_obj_set_style_text_color(outcome_lbl, C_TEXT, 0);
    lv_obj_set_style_text_font(outcome_lbl, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(outcome_lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(outcome_lbl, 200);
    lv_obj_align(outcome_lbl, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_text_align(outcome_lbl, LV_TEXT_ALIGN_CENTER, 0);

    /* Consent-Overlay */
    consent_overlay = lv_obj_create(scr);
    lv_obj_set_size(consent_overlay, 240, 240);
    lv_obj_align(consent_overlay, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(consent_overlay, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(consent_overlay, LV_OPA_70, 0);
    lv_obj_set_style_border_width(consent_overlay, 0, 0);
    lv_obj_set_style_radius(consent_overlay, 0, 0);
    lv_obj_clear_flag(consent_overlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(consent_overlay, LV_OBJ_FLAG_HIDDEN);

    /* Consent-Card (innerhalb des Overlays) */
    lv_obj_t *ccard = lv_obj_create(consent_overlay);
    lv_obj_set_size(ccard, 180, 140);
    lv_obj_align(ccard, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(ccard, C_CARD, 0);
    lv_obj_set_style_border_color(ccard, C_PURPLE, 0);
    lv_obj_set_style_border_width(ccard, 1, 0);
    lv_obj_set_style_radius(ccard, 12, 0);
    lv_obj_clear_flag(ccard, LV_OBJ_FLAG_SCROLLABLE);

    consent_room_lbl = lv_label_create(ccard);
    lv_label_set_text(consent_room_lbl, "Agent möchte:");
    lv_obj_set_style_text_color(consent_room_lbl, C_TEXT, 0);
    lv_obj_set_style_text_font(consent_room_lbl, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(consent_room_lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(consent_room_lbl, 160);
    lv_obj_set_style_text_align(consent_room_lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(consent_room_lbl, LV_ALIGN_TOP_MID, 0, 14);

    /* Erlauben-Button */
    consent_allow_btn = lv_btn_create(ccard);
    lv_obj_set_size(consent_allow_btn, 70, 34);
    lv_obj_align(consent_allow_btn, LV_ALIGN_BOTTOM_LEFT, 8, -10);
    lv_obj_set_style_bg_color(consent_allow_btn, C_PURPLE_BG, 0);
    lv_obj_set_style_border_color(consent_allow_btn, C_PURPLE, 0);
    lv_obj_set_style_border_width(consent_allow_btn, 1, 0);
    lv_obj_set_style_radius(consent_allow_btn, 8, 0);
    lv_obj_add_event_cb(consent_allow_btn, on_allow, LV_EVENT_CLICKED, NULL);
    lv_obj_t *allow_lbl = lv_label_create(consent_allow_btn);
    lv_label_set_text(allow_lbl, "OK");
    lv_obj_set_style_text_color(allow_lbl, C_TEXT, 0);
    lv_obj_set_style_text_font(allow_lbl, &lv_font_montserrat_12, 0);
    lv_obj_center(allow_lbl);

    /* Ablehnen-Button */
    consent_deny_btn = lv_btn_create(ccard);
    lv_obj_set_size(consent_deny_btn, 70, 34);
    lv_obj_align(consent_deny_btn, LV_ALIGN_BOTTOM_RIGHT, -8, -10);
    lv_obj_set_style_bg_color(consent_deny_btn, lv_color_hex(0x2a1818), 0);
    lv_obj_set_style_border_color(consent_deny_btn, lv_color_hex(0x791f1f), 0);
    lv_obj_set_style_border_width(consent_deny_btn, 1, 0);
    lv_obj_set_style_radius(consent_deny_btn, 8, 0);
    lv_obj_add_event_cb(consent_deny_btn, on_deny, LV_EVENT_CLICKED, NULL);
    lv_obj_t *deny_lbl = lv_label_create(consent_deny_btn);
    lv_label_set_text(deny_lbl, "Nein");
    lv_obj_set_style_text_color(deny_lbl, lv_color_hex(0xf09595), 0);
    lv_obj_set_style_text_font(deny_lbl, &lv_font_montserrat_12, 0);
    lv_obj_center(deny_lbl);
}

/* ---- State-Updates verarbeiten ---- */

void AgentUI_handle(const sp_message_t *msg) {
    int i;

    switch (msg->type) {

        case SP_MSG_START:
            /* Alle Cards zurücksetzen */
            for (i = 0; i < 3; i++) reset_card(i);
            lv_label_set_text(outcome_lbl, "");
            lv_obj_add_flag(consent_overlay, LV_OBJ_FLAG_HIDDEN);
            /* Modus anzeigen */
            switch (msg->mode) {
                case SP_MODE_HITL:       lv_label_set_text(mode_lbl, "Human-in-the-Loop"); break;
                case SP_MODE_AUTO:       lv_label_set_text(mode_lbl, "Auto + Hooks");       break;
                case SP_MODE_DESTRUKTIV: lv_label_set_text(mode_lbl, "Destruktiv");         break;
                default:                 lv_label_set_text(mode_lbl, "–");                  break;
            }
            break;

        case SP_MSG_APPROACHING:
            i = room_index(msg->room);
            if (i < 0) break;
            set_dot_color(i, C_YELLOW);
            set_card_border(i, C_YELLOW);
            lv_label_set_text(status_lbl[i], "Nähert sich...");
            break;

        case SP_MSG_CONSENT_NEEDED:
            i = room_index(msg->room);
            if (i < 0) break;
            set_dot_color(i, C_PURPLE);
            set_card_border(i, C_PURPLE);
            lv_label_set_text(status_lbl[i], "Warte...");
            /* Consent-Overlay zeigen */
            lv_label_set_text(consent_room_lbl, ROOM_LABELS[i]);
            lv_obj_clear_flag(consent_overlay, LV_OBJ_FLAG_HIDDEN);
            break;

        case SP_MSG_ALLOWED:
            i = room_index(msg->room);
            if (i < 0) break;
            lv_obj_add_flag(consent_overlay, LV_OBJ_FLAG_HIDDEN);
            set_dot_color(i, C_GREEN);
            lv_label_set_text(status_lbl[i], "Erlaubt");
            break;

        case SP_MSG_DENIED:
            i = room_index(msg->room);
            if (i < 0) break;
            lv_obj_add_flag(consent_overlay, LV_OBJ_FLAG_HIDDEN);
            set_card_border(i, lv_color_hex(0x501313));
            set_dot_color(i, lv_color_hex(0x701f1f));
            lv_obj_set_style_opa(card[i], LV_OPA_50, 0);
            lv_label_set_text(status_lbl[i], "Blockiert");
            break;

        case SP_MSG_ENTER:
            i = room_index(msg->room);
            if (i < 0) break;
            set_dot_color(i, sp_color_to_lv(msg->color));
            set_card_border(i, sp_color_to_lv(msg->color));
            lv_label_set_text(status_lbl[i], "Betreten");
            break;

        case SP_MSG_BETRAYAL:
            i = room_index(msg->room);
            if (i < 0) break;
            set_dot_color(i, C_RED);
            set_card_border(i, C_RED);
            lv_label_set_text(status_lbl[i], "BRUCH!");
            break;

        case SP_MSG_DONE:
            switch (msg->outcome) {
                case SP_OUTCOME_SUCCESS:
                    lv_label_set_text(outcome_lbl, LV_SYMBOL_OK " Termin vereinbart");
                    lv_obj_set_style_text_color(outcome_lbl, C_GREEN, 0);
                    break;
                case SP_OUTCOME_BETRAYAL:
                    lv_label_set_text(outcome_lbl, LV_SYMBOL_WARNING " Vertrauensbruch");
                    lv_obj_set_style_text_color(outcome_lbl, C_RED, 0);
                    break;
                case SP_OUTCOME_BLOCKED:
                    lv_label_set_text(outcome_lbl, LV_SYMBOL_CLOSE " Blockiert");
                    lv_obj_set_style_text_color(outcome_lbl, C_SUBTEXT, 0);
                    break;
                default: break;
            }
            break;

        default:
            break;
    }
}
