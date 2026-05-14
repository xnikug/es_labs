#include "lab6_2Traffic.h"
#include "lab6_2Shared.h"
#include "lab6_2Fsm.h"
#include <Arduino_FreeRTOS.h>

void vTask6_2Traffic(void *pvParams) {
    (void)pvParams;
    TlState_t state = TL_EAST_GREEN;

    for (;;) {
        lab6_2SharedSetState(state);
        lab6_2FsmApplyOutputs(state);

        if (state == TL_EAST_GREEN) {
            xSemaphoreTake(g_northReqSem, portMAX_DELAY);
        } else {
            vTaskDelay(pdMS_TO_TICKS(lab6_2FsmTable[state].delayMs));
        }

        state = lab6_2FsmTable[state].nextState;
    }
}
