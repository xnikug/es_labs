#include "lab6_2Report.h"
#include "lab6_2Shared.h"
#include "lab6_2Fsm.h"
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

void vTask6_2Report(void *pvParams) {
    (void)pvParams;
    TickType_t xLastWake = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWake, pdMS_TO_TICKS(500));
        TlState_t s = lab6_2SharedGetState();
        printf("[TL] State:%u %s\n", (unsigned)s, lab6_2StateNames[s]);
    }
}
