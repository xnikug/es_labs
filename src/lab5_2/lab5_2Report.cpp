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
    uint8_t lcdReady = 0U;

    static char t[6], sp[6];
    static char line1[17], line2[17];

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_2_TASK_REPORT_MS));

        SensorData_t   sensor;
        ControlState_t ctrl;
        RelayState_t   relay;
        appSharedGetSensor(&sensor);
        appSharedGetControl(&ctrl);
        appSharedGetRelay(&relay);

        dtostrf(ctrl.setpointC, 4, 1, sp);
        if (sensor.valid)
            dtostrf(sensor.temperatureC, 4, 1, t);
        else
            memcpy(t, "----", 5);

        unsigned pct    = (unsigned)(ctrl.pidOutput * 100.0f + 0.5f);
        const char* fan = relay.isOn ? "ON" : "OFF";

        snprintf(line1, 17, "T:%s SP:%s", t, sp);
        snprintf(line2, 17, "PID:%3u%%  Fan:%s", pct, fan);

        if (lcdReady == 0U) { ddLcdClear(); lcdReady = 1U; }
        ddLcdSetCursor(0, 0);
        ddLcdPrint(line1);
        ddLcdSetCursor(0, 1);
        ddLcdPrint(line2);
    }
}
