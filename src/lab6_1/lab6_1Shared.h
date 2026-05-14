#pragma once
#include <stdint.h>
#include "../app_config.h"

/* ── Task period ────────────────────────────────────────────────── */
#define LAB6_1_FSM_PERIOD_MS    APP_LAB6_1_FSM_PERIOD_MS
#define LAB6_1_DEBOUNCE_MS      APP_LAB6_1_DEBOUNCE_MS
#define LAB6_1_BTN_PIN          APP_LAB6_1_BTN_PIN
#define LAB6_1_LED_PIN          APP_LAB6_1_LED_PIN

/* ── FSM state indices ──────────────────────────────────────────── */
#define LED_OFF_STATE       0U
#define LED_ON_STATE        1U
#define LAB6_1_NUM_STATES   2U

/* ── Moore FSM state descriptor ─────────────────────────────────── */
typedef struct {
    uint8_t  out;           /* LED pin value: 0=off, 1=on            */
    uint16_t delayMs;       /* state hold period (ms)                */
    uint8_t  next[2];       /* next[0]=no-press, next[1]=press event */
} Lab6_1State_t;

/* FSM lookup table — defined in lab6_1Shared.cpp */
extern const Lab6_1State_t  lab6_1FsmTable[LAB6_1_NUM_STATES];
extern const char *const    lab6_1StateNames[LAB6_1_NUM_STATES];
