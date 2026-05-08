#include "lab5_2Report.h"
#include "lab5_2Shared.h"
#include "../ddLcd/ddLcd.h"

#include <Arduino_FreeRTOS.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vTask5_2Report(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t lcd_ready = 0U;

    static char t[6], sp[6];
    static char line1[17], line2[17];

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_2_TASK_REPORT_MS));

        sensor_data_t   sensor;
        control_state_t ctrl;
        relay_state_t   relay;
        app_shared_get_sensor(&sensor);
        app_shared_get_control(&ctrl);
        app_shared_get_relay(&relay);

        dtostrf(ctrl.setpoint_c, 4, 1, sp);
        if (sensor.valid)
            dtostrf(sensor.temperature_c, 4, 1, t);
        else
            memcpy(t, "----", 5);

        unsigned pct    = (unsigned)(ctrl.pid_output * 100.0f + 0.5f);
        const char* fan = relay.is_on ? "ON" : "OFF";

        snprintf(line1, 17, "T:%s SP:%s", t, sp);
        snprintf(line2, 17, "PID:%3u%%  Fan:%s", pct, fan);

        if (lcd_ready == 0U) {
            ddLcdClear();
            lcd_ready = 1U;
        }
        ddLcdSetCursor(0, 0);
        ddLcdPrint(line1);
        ddLcdSetCursor(0, 1);
        ddLcdPrint(line2);
    }
}
