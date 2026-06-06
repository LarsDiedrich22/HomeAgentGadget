#include "SerialProtocol.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Zeilenpuffer */
#define LINE_BUF_SIZE 128
static char  s_line[LINE_BUF_SIZE];
static int   s_pos = 0;

/* ---- Hilfsfunktionen ---- */

static sp_color_t parse_color(const char *s) {
    if (strcmp(s, "green")  == 0) return SP_COLOR_GREEN;
    if (strcmp(s, "yellow") == 0) return SP_COLOR_YELLOW;
    if (strcmp(s, "red")    == 0) return SP_COLOR_RED;
    return SP_COLOR_NONE;
}

static sp_mode_t parse_mode(const char *s) {
    if (strcmp(s, "hitl")       == 0) return SP_MODE_HITL;
    if (strcmp(s, "auto")       == 0) return SP_MODE_AUTO;
    if (strcmp(s, "destruktiv") == 0) return SP_MODE_DESTRUKTIV;
    return SP_MODE_NONE;
}

static sp_outcome_t parse_outcome(const char *s) {
    if (strcmp(s, "success")  == 0) return SP_OUTCOME_SUCCESS;
    if (strcmp(s, "blocked")  == 0) return SP_OUTCOME_BLOCKED;
    if (strcmp(s, "betrayal") == 0) return SP_OUTCOME_BETRAYAL;
    return SP_OUTCOME_NONE;
}

static bool parse_line(const char *line, sp_message_t *msg) {
    memset(msg, 0, sizeof(*msg));

    char cmd[32], a1[32], a2[32], a3[32];
    int n = sscanf(line, "%31s %31s %31s %31s", cmd, a1, a2, a3);
    if (n < 1) return false;

    if (strcmp(cmd, "START") == 0) {
        msg->type = SP_MSG_START;
        if (n >= 2) msg->mode = parse_mode(a1);
        return true;
    }
    if (strcmp(cmd, "APPROACHING") == 0) {
        msg->type = SP_MSG_APPROACHING;
        if (n >= 2) strncpy(msg->room, a1, sizeof(msg->room) - 1);
        if (n >= 3) msg->color = parse_color(a2);
        return true;
    }
    if (strcmp(cmd, "CONSENT_NEEDED") == 0) {
        msg->type = SP_MSG_CONSENT_NEEDED;
        if (n >= 2) strncpy(msg->room, a1, sizeof(msg->room) - 1);
        if (n >= 4) msg->color = parse_color(a3);
        return true;
    }
    if (strcmp(cmd, "ALLOWED") == 0) {
        msg->type = SP_MSG_ALLOWED;
        if (n >= 2) strncpy(msg->room, a1, sizeof(msg->room) - 1);
        return true;
    }
    if (strcmp(cmd, "DENIED") == 0) {
        msg->type = SP_MSG_DENIED;
        if (n >= 2) strncpy(msg->room, a1, sizeof(msg->room) - 1);
        return true;
    }
    if (strcmp(cmd, "ENTER") == 0) {
        msg->type = SP_MSG_ENTER;
        if (n >= 2) strncpy(msg->room, a1, sizeof(msg->room) - 1);
        if (n >= 3) msg->color = parse_color(a2);
        return true;
    }
    if (strcmp(cmd, "BETRAYAL") == 0) {
        msg->type = SP_MSG_BETRAYAL;
        if (n >= 2) strncpy(msg->room, a1, sizeof(msg->room) - 1);
        return true;
    }
    if (strcmp(cmd, "DONE") == 0) {
        msg->type = SP_MSG_DONE;
        if (n >= 2) msg->outcome = parse_outcome(a1);
        if (n >= 3) strncpy(msg->denied_at, a2, sizeof(msg->denied_at) - 1);
        return true;
    }
    return false;
}

/* ---- Öffentliche API ---- */

bool SerialProtocol_poll(sp_message_t *msg) {
    int c;
    while ((c = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT) {
        if (c == '\r') continue;
        if (c == '\n') {
            s_line[s_pos] = '\0';
            int len = s_pos;
            s_pos = 0;
            if (len > 0) {
                return parse_line(s_line, msg);
            }
            return false;
        }
        if (s_pos < LINE_BUF_SIZE - 1) {
            s_line[s_pos++] = (char)c;
        }
    }
    return false;
}

void SerialProtocol_send_consent(bool allow) {
    printf("CONSENT %s\n", allow ? "allow" : "deny");
}
