#pragma once
#include "lab6_2Shared.h"
#include <stdint.h>

#define TL_LED_OFF    0U
#define TL_LED_GREEN  1U
#define TL_LED_YELLOW 2U
#define TL_LED_RED    3U

typedef struct {
    uint8_t   eastLed;
    uint8_t   northLed;
    uint16_t  delayMs;
    TlState_t nextState;
} TlFsmState_t;

extern const TlFsmState_t lab6_2FsmTable[TL_STATE_COUNT];
extern const char *const  lab6_2StateNames[TL_STATE_COUNT];

void lab6_2FsmInitPins(void);
void lab6_2FsmApplyOutputs(TlState_t state);
