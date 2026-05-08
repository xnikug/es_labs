#include "lab5_1Actuation.h"
#include "lab5_1Shared.h"
#include "../drivers/motor_stdio/motor_stdio.h"
#include "../drivers/led_stdio/led_stdio.h"

#include <Arduino_FreeRTOS.h>

void vTask5_1Actuation(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    FILE* g_led = led_stdio_stream();

    static RelayCmd_t s_cur     = RELAY_CMD_OFF;
    static uint32_t   s_since   = 0UL;
    static uint32_t   s_onCyc   = 0UL;
    static uint32_t   s_offCyc  = 0UL;

    for (;;) {
        uint32_t       now = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
        ControlState_t ctrl;
        appSharedGetControl(&ctrl);
        RelayCmd_t target = ctrl.onoffOutput;

        if (target != s_cur) {
            s_cur = target;
            s_since = now;
            if (s_cur == RELAY_CMD_ON) s_onCyc++; else s_offCyc++;
        }

        motor_set_state(s_cur == RELAY_CMD_ON ? 1U : 0U);
        fputc(s_cur == RELAY_CMD_ON ? '1' : '0', g_led);

        RelayState_t rs;
        rs.isOn      = (s_cur == RELAY_CMD_ON) ? 1U : 0U;
        rs.onCycles  = s_onCyc;
        rs.offCycles = s_offCyc;
        rs.stableMs  = (uint32_t)(now - s_since);
        appSharedSetRelay(&rs);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_1_TASK_ACT_MS));
    }
}
