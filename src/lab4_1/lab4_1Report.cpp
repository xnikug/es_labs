/**
 * @file lab4_1Report.cpp
 * @brief Report task for Lab 4.1.
 */

#include "lab4_1Report.h"
#include "lab4_1Shared.h"
#include "../ddLcd/ddLcd.h"
#include <stdio.h>

static const char *cmdToText(bool on)
{
    return on ? "ON " : "OFF";
}

void vTask4_1Report(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(1000));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(500);

    for (;;) {
        Lab4_1State_t state = lab4_1SharedGetSnapshot();

        const char *statusText = (state.debounceCounter == 0) ? "STBL" : "WAIT";

        char line0[17];
        char line1[17];
        snprintf(line0, sizeof(line0), "RAW %-3s DB %-3s", cmdToText(state.rawRequestedOn), cmdToText(state.relayRequestedOn));
        snprintf(line1, sizeof(line1), "CYC %2u/%-2d %-4s", state.debounceCounter, LAB4_1_DEBOUNCE_MAX_COUNTER, statusText);

        ddLcdClear();
        ddLcdSetCursor(0, 0);
        ddLcdPrint(line0);
        ddLcdSetCursor(0, 1);
        ddLcdPrint(line1);

        lab4_1SharedConsumeAlertCycle();
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
