#include "lab5_1Report.h"
#include "lab5_1Shared.h"
#include "../ddLcd/ddLcd.h"

#include <Arduino_FreeRTOS.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vTask5_1Report(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t lcdReady = 0U;

    static char t[6], sp[6], hyst[6];
    static char line1[17], line2[17];

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_1_TASK_REP_MS));

        SensorData_t   sensor;
        ControlState_t ctrl;
        RelayState_t   relay;
        appSharedGetSensor(&sensor);
        appSharedGetControl(&ctrl);
        appSharedGetRelay(&relay);

        dtostrf(ctrl.setpointC,   4, 1, sp);
        dtostrf(ctrl.hysteresisC, 4, 2, hyst);
        if (sensor.valid)
            dtostrf(sensor.temperatureC, 4, 1, t);
        else
            memcpy(t, "----", 5);

        const char* fan = relay.isOn ? "ON" : "OFF";

        snprintf(line1, 17, "T:%s SP:%s", t, sp);
        snprintf(line2, 17, "H:%s  Fan:%s", hyst, fan);

        if (lcdReady == 0U) { ddLcdClear(); lcdReady = 1U; }
        ddLcdSetCursor(0, 0);
        ddLcdPrint(line1);
        ddLcdSetCursor(0, 1);
        ddLcdPrint(line2);
    }
}
