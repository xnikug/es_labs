#include "lab6_1Shared.h"

/*
 * Moore FSM lookup table for Button-LED.
 *
 * | Num | Name    | Out | Delay   | In=0      | In=1     |
 * |-----|---------|-----|---------|-----------|----------|
 * |  0  | LED_OFF |  0  | 100 ms  | LED_OFF   | LED_ON   |
 * |  1  | LED_ON  |  1  | 100 ms  | LED_ON    | LED_OFF  |
 */
const Lab6_1State_t lab6_1FsmTable[LAB6_1_NUM_STATES] = {
    /* State 0: LED_OFF */
    { 0U, LAB6_1_FSM_PERIOD_MS, { LED_OFF_STATE, LED_ON_STATE  } },
    /* State 1: LED_ON  */
    { 1U, LAB6_1_FSM_PERIOD_MS, { LED_ON_STATE,  LED_OFF_STATE } }
};

const char *const lab6_1StateNames[LAB6_1_NUM_STATES] = {
    "LED_OFF", "LED_ON"
};
