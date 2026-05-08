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

    static relay_cmd_t s_cur     = RELAY_CMD_OFF;
    static uint32_t    s_since   = 0UL;
    static uint32_t    s_on_cyc  = 0UL;
    static uint32_t    s_off_cyc = 0UL;

    for (;;) {
        uint32_t        now = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
        control_state_t ctrl;
        app_shared_get_control(&ctrl);
        relay_cmd_t target = ctrl.onoff_output;

        if (target != s_cur) {
            s_cur = target;
            s_since = now;
            if (s_cur == RELAY_CMD_ON) s_on_cyc++; else s_off_cyc++;
        }

        motor_set_state(s_cur == RELAY_CMD_ON ? 1U : 0U);
        fputc(s_cur == RELAY_CMD_ON ? '1' : '0', g_led);

        relay_state_t rs;
        rs.is_on      = (s_cur == RELAY_CMD_ON) ? 1U : 0U;
        rs.on_cycles  = s_on_cyc;
        rs.off_cycles = s_off_cyc;
        rs.stable_ms  = (uint32_t)(now - s_since);
        app_shared_set_relay(&rs);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_1_TASK_ACT_MS));
    }
}
