#include "lab6_1Fsm.h"
#include "lab6_1Shared.h"
#include <Arduino.h>

static uint8_t  s_state     = LED_OFF_STATE;
static uint8_t  s_rawLast   = HIGH;   /* INPUT_PULLUP idle = HIGH */
static uint8_t  s_debounced = HIGH;
static uint32_t s_changeMs  = 0UL;
static uint8_t  s_prevDeb   = HIGH;

void lab6_1FsmInit(void) {
    pinMode(LAB6_1_BTN_PIN, INPUT_PULLUP);
    pinMode(LAB6_1_LED_PIN, OUTPUT);
    digitalWrite(LAB6_1_LED_PIN, LOW);
    s_state     = LED_OFF_STATE;
    s_rawLast   = HIGH;
    s_debounced = HIGH;
    s_prevDeb   = HIGH;
    s_changeMs  = 0UL;
}

/*
 * Software debounce + falling-edge detection (active-LOW button).
 * Must be called frequently (every loop iteration) for correct debounce.
 * Returns 1 exactly once per validated button press; 0 otherwise.
 */
uint8_t lab6_1FsmPollButton(void) {
    uint8_t  raw = digitalRead(LAB6_1_BTN_PIN);
    uint32_t now = millis();

    if (raw != s_rawLast) {
        s_rawLast  = raw;
        s_changeMs = now;
    }
    if ((uint32_t)(now - s_changeMs) >= LAB6_1_DEBOUNCE_MS) {
        s_debounced = s_rawLast;
    }

    /* Falling edge on debounced signal = button pressed */
    uint8_t edge = (s_prevDeb == HIGH && s_debounced == LOW) ? 1U : 0U;
    s_prevDeb = s_debounced;
    return edge;
}

/* Step 3 snapshot read: returns 1 if button is pressed (LOW) at this instant.
 * Used with the blocking 4-step loop where delay() in step 2 already
 * provides the debounce settling window. */
uint8_t lab6_1FsmReadInput(void) {
    return (digitalRead(LAB6_1_BTN_PIN) == LOW) ? 1U : 0U;
}

/* Moore output: drive LED based solely on current state */
void lab6_1FsmApplyOutput(void) {
    digitalWrite(LAB6_1_LED_PIN, lab6_1FsmTable[s_state].out);
}

/* Transition: nextState = FSM[currentState].next[input] */
void lab6_1FsmStep(uint8_t input) {
    s_state = lab6_1FsmTable[s_state].next[input & 1U];
}

uint8_t  lab6_1FsmGetState(void) { return s_state; }
uint16_t lab6_1FsmGetDelay(void) { return lab6_1FsmTable[s_state].delayMs; }
