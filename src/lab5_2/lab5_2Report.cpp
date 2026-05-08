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

#if LAB5_2_OUTPUT_MODE == LAB5_2_MODE_MONITOR
    uint8_t lcdReady = 0U;
    static char t[6], sp[6];
    static char line1[17], line2[17];
#else
    /* Four 8-char buffers for the four plotter traces */
    static char s_sp[8], s_t[8], s_dt[8], s_u[8];
#endif

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_2_TASK_REPORT_MS));

        SensorData_t   sensor;
        ControlState_t ctrl;
        RelayState_t   relay;
        appSharedGetSensor(&sensor);
        appSharedGetControl(&ctrl);
        appSharedGetRelay(&relay);

#if LAB5_2_OUTPUT_MODE == LAB5_2_MODE_PLOTTER
        /*
         * All four signals are normalised into the same temperature band
         * so they share one Y-axis and remain visually readable together.
         *
         *  SP  – setpoint (°C) ............. flat reference line
         *  T   – measured temperature (°C) . should track SP
         *  dT  – error×5 + SP .............. sits on SP when error=0;
         *                                    rises/falls ±5°C per 1°C error
         *  u   – PID×20 + SP−10 ............ SP−10 at 0%, SP at 50%, SP+10 at 100%
         *
         * Width argument of 1 = no leading-space padding (SP:23.50 not SP: 23.50).
         */
        float tVal = sensor.valid ? sensor.temperatureC : ctrl.setpointC;
        float sp   = ctrl.setpointC;
        float dt_n = (tVal - sp) * 5.0f + sp;          /* error amplified, °C */
        float u_n  = ctrl.pidOutput * 20.0f + sp - 10.0f; /* duty mapped, °C  */

        dtostrf(sp,   1, 2, s_sp);
        dtostrf(tVal, 1, 2, s_t);
        dtostrf(dt_n, 1, 2, s_dt);
        dtostrf(u_n,  1, 2, s_u);
        printf(">SP:%s,T:%s,dT:%s,u:%s\n", s_sp, s_t, s_dt, s_u);
#else
        /* Serial Monitor + LCD */
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
#endif
    }
}
