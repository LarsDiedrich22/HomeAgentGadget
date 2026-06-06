#pragma once
#include "SerialProtocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Erstellt alle LVGL-Screens und Widgets für die Agent-Trust-UI.
 * Muss nach lv_init() und DisplayInit() aufgerufen werden.
 *
 * consent_cb wird aufgerufen wenn der User im Consent-Dialog
 * auf Erlauben (allow=true) oder Ablehnen (allow=false) tippt.
 * Der Caller sendet dann SerialProtocol_send_consent().
 */
void AgentUI_init(void (*consent_cb)(bool allow));

/**
 * Verarbeitet eine empfangene Serial-Nachricht und aktualisiert die UI.
 */
void AgentUI_handle(const sp_message_t *msg);

#ifdef __cplusplus
}
#endif
