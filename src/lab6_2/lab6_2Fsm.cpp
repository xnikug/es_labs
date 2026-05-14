#include "lab6_2Fsm.h"
#include "../app_config.h"
#include <Arduino.h>

const TlFsmState_t lab6_2FsmTable[TL_STATE_COUNT] = {
    { TL_LED_GREEN,  TL_LED_RED,    0U,                      TL_EAST_YELLOW  },
    { TL_LED_YELLOW, TL_LED_RED,    APP_TL_YELLOW_MS,        TL_ALL_RED_A    },
    { TL_LED_RED,    TL_LED_RED,    APP_TL_ALL_RED_MS,       TL_NORTH_GREEN  },
    { TL_LED_RED,    TL_LED_GREEN,  APP_TL_NORTH_GREEN_MS,   TL_NORTH_YELLOW },
    { TL_LED_RED,    TL_LED_YELLOW, APP_TL_YELLOW_MS,        TL_ALL_RED_B    },
    { TL_LED_RED,    TL_LED_RED,    APP_TL_ALL_RED_MS,       TL_EAST_GREEN   }
};

const char *const lab6_2StateNames[TL_STATE_COUNT] = {
    "EAST_GREEN", "EAST_YELLOW", "ALL_RED_A",
    "NORTH_GREEN", "NORTH_YELLOW", "ALL_RED_B"
};

static void applyColour(uint8_t gPin, uint8_t yPin, uint8_t rPin, uint8_t colour) {
    digitalWrite(gPin, colour == TL_LED_GREEN  ? HIGH : LOW);
    digitalWrite(yPin, colour == TL_LED_YELLOW ? HIGH : LOW);
    digitalWrite(rPin, colour == TL_LED_RED    ? HIGH : LOW);
}

void lab6_2FsmInitPins(void) {
    uint8_t outPins[6] = {
        APP_LAB6_2_EAST_GREEN_PIN,  APP_LAB6_2_EAST_YELLOW_PIN,  APP_LAB6_2_EAST_RED_PIN,
        APP_LAB6_2_NORTH_GREEN_PIN, APP_LAB6_2_NORTH_YELLOW_PIN, APP_LAB6_2_NORTH_RED_PIN
    };
    for (uint8_t i = 0; i < 6U; i++) {
        pinMode(outPins[i], OUTPUT);
        digitalWrite(outPins[i], LOW);
    }
    pinMode(APP_LAB6_2_REQ_BTN_PIN, INPUT_PULLUP);
}

void lab6_2FsmApplyOutputs(TlState_t state) {
    const TlFsmState_t *s = &lab6_2FsmTable[state];
    applyColour(APP_LAB6_2_EAST_GREEN_PIN,  APP_LAB6_2_EAST_YELLOW_PIN,  APP_LAB6_2_EAST_RED_PIN,  s->eastLed);
    applyColour(APP_LAB6_2_NORTH_GREEN_PIN, APP_LAB6_2_NORTH_YELLOW_PIN, APP_LAB6_2_NORTH_RED_PIN, s->northLed);
}
