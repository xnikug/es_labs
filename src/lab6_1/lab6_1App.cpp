#include "lab6_1App.h"
#include "lab6_1Fsm.h"
#include "lab6_1Shared.h"
#include "../app_config.h"
#include "../drivers/uart_stdio/uart_stdio.h"
#include <Arduino.h>

void lab6_1AppSetup(void) {
    uart_stdio_init(APP_UART_BAUD);
    lab6_1FsmInit();

    printf("\n=== LAB 6.1 BUTTON-LED FSM ===\n");
    printf("Button=D%u (pull-up, active-LOW)  LED=D%u\n",
           (unsigned)LAB6_1_BTN_PIN, (unsigned)LAB6_1_LED_PIN);
    printf("Press button to toggle LED state.\n\n");
}

void lab6_1AppLoop(void) {
    uint8_t st = lab6_1FsmGetState();

    /* Step 1 — Moore output: LED = f(current state) */
    lab6_1FsmApplyOutput();

    /* Step 2+3 — Wait and detect a button edge within the window.
     * Polling PollButton() during the delay gives proper edge detection:
     * holding the button only triggers one transition, not one per cycle. */
    uint8_t  input = 0;
    uint32_t end   = millis() + lab6_1FsmGetDelay();
    while ((int32_t)(end - millis()) > 0) {
        if (lab6_1FsmPollButton()) { input = 1; }
        delay(1);
    }

    /* Step 4 — Change State: transition = f(currentState, input) */
    lab6_1FsmStep(input);
    uint8_t currState = lab6_1FsmGetState();

    if (currState != st) {
        printf("[FSM] %s -> %s  LED=%s\n",
               lab6_1StateNames[st],
               lab6_1StateNames[currState],
               lab6_1FsmTable[currState].out ? "ON" : "OFF");
    }
}
