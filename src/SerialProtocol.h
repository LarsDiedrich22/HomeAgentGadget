#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Protokoll: HomeAgent-Server -> RP2040 über USB-Serial (stdio)
 *
 * Jede Nachricht ist eine einzelne Zeile (LF-terminiert), z.B.:
 *
 *   START hitl
 *   APPROACHING kalender green
 *   CONSENT_NEEDED kalender "Kalender lesen" green
 *   ALLOWED kalender
 *   DENIED kalender
 *   ENTER kalender green
 *   BETRAYAL kalender
 *   DONE success
 *   DONE blocked kalender
 *   DONE betrayal
 *
 * Antworten vom RP2040 -> PC (für Consent):
 *   CONSENT allow
 *   CONSENT deny
 */

typedef enum {
    SP_MSG_NONE = 0,
    SP_MSG_START,
    SP_MSG_APPROACHING,
    SP_MSG_CONSENT_NEEDED,
    SP_MSG_ALLOWED,
    SP_MSG_DENIED,
    SP_MSG_ENTER,
    SP_MSG_BETRAYAL,
    SP_MSG_DONE,
} sp_msg_type_t;

typedef enum {
    SP_COLOR_NONE = 0,
    SP_COLOR_GREEN,
    SP_COLOR_YELLOW,
    SP_COLOR_RED,
} sp_color_t;

typedef enum {
    SP_MODE_NONE = 0,
    SP_MODE_HITL,
    SP_MODE_AUTO,
    SP_MODE_DESTRUKTIV,
} sp_mode_t;

typedef enum {
    SP_OUTCOME_NONE = 0,
    SP_OUTCOME_SUCCESS,
    SP_OUTCOME_BLOCKED,
    SP_OUTCOME_BETRAYAL,
} sp_outcome_t;

typedef struct {
    sp_msg_type_t type;
    char          room[16];     /* "kalender" | "kontakte" | "mail" */
    sp_color_t    color;
    sp_mode_t     mode;
    sp_outcome_t  outcome;
    char          denied_at[16];
} sp_message_t;

/**
 * Nicht-blockierendes Poll: liest eine vollständige Zeile aus stdin,
 * parst sie und füllt msg. Gibt true zurück wenn eine Nachricht vorliegt.
 */
bool SerialProtocol_poll(sp_message_t *msg);

/**
 * Sendet eine Consent-Entscheidung an den PC.
 * allow=true -> "CONSENT allow\n", allow=false -> "CONSENT deny\n"
 */
void SerialProtocol_send_consent(bool allow);

#ifdef __cplusplus
}
#endif
