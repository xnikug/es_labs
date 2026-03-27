/**
 * @file lab4_1Actuation.cpp
 * @brief Relay actuation task for Lab 4.1.
 */

#include "lab4_1Actuation.h"
#include "lab4_1Shared.h"
#include "../ddFan/ddFan.h"

void vTask4_1Actuation(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(700));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(100);

    for (;;) {
        Lab4_1State_t state = lab4_1SharedGetSnapshot();

        if (state.relayRequestedOn) {
            ddFanOn();
        } else {
            ddFanOff();
        }

        lab4_1SharedSetAppliedRelayState(ddFanIsOn());
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
