/**
 * @file lab4_1Conditioning.cpp
 * @brief Command conditioning task (debounce/persistence) for Lab 4.1.
 */

#include "lab4_1Conditioning.h"
#include "lab4_1Shared.h"

#define LAB4_1_COND_PERIOD_MS 100

void vTask4_1Conditioning(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(650));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(LAB4_1_COND_PERIOD_MS);

    bool debouncedCommand = LAB4_1_FAN_CMD_OFF;
    uint8_t debounceCounter = 0;

    for (;;) {
        const Lab4_1State_t state = lab4_1SharedGetSnapshot();
        const bool rawCommand = state.rawRequestedOn;

        if (rawCommand == debouncedCommand) {
            debounceCounter = 0;
        } else {
            if (debounceCounter < LAB4_1_DEBOUNCE_MAX_COUNTER) {
                debounceCounter++;
            }

            if (debounceCounter >= LAB4_1_DEBOUNCE_MAX_COUNTER) {
                debouncedCommand = rawCommand;
                debounceCounter = 0;
            }
        }

        lab4_1SharedSetDebouncedCommand(debouncedCommand, debounceCounter);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
