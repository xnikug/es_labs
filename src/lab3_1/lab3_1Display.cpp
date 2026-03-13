#include "lab3_1Display.h"
#include "lab3_1Shared.h"
#include "../ddLcd/ddLcd.h"
#include <stdio.h>

static float gLastValidTemperatureC = 0.0f;

static float getDisplayTemperature(const SensorReadings_t &sample)
{
    if (sample.temperatureValid) {
        gLastValidTemperatureC = sample.temperatureC;
    }

    return gLastValidTemperatureC;
}

static void renderLcdLines(const SensorReadings_t &sample, char *line0, size_t line0Size, char *line1, size_t line1Size)
{
    const float displayTempC = getDisplayTemperature(sample);
    const int centeredAngle = sample.angleRawDeg - LAB3_1_ANGLE_OFFSET_DEG;
    const int intPart = (int)displayTempC;
    int fracPart = (int)((displayTempC - (float)intPart) * 100.0f);
    if (fracPart < 0) {
        fracPart = -fracPart;
    }

    snprintf(line0, line0Size, "T:%d.%02d A:%+3d", intPart, fracPart, centeredAngle);

    if (sample.angleAlertState == LAB3_1_ALERT_ACTIVE) {
        snprintf(line1, line1Size, "AlertA     D=%d", sample.angleDebounceCounter);
    } else {
        snprintf(line1, line1Size, "OkA        D=%d", sample.angleDebounceCounter);
    }
}

void lab3_1DisplayInit() {
    ddLcdInit();
}

void vTaskDisplay(void *pvParameters) {
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(1000));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_DISPLAY_PERIOD_MS);

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        SensorReadings_t lSens;
        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(20)) == pdTRUE) {
            lSens = g_sensorData;
            xSemaphoreGive(xSensorMutex);
        } else {
            continue;
        }

        char line0[17];
        char line1[17];
        renderLcdLines(lSens, line0, sizeof(line0), line1, sizeof(line1));

        ddLcdClear();
        ddLcdSetCursor(0, 0);
        ddLcdPrint(line0);
        ddLcdSetCursor(0, 1);
        ddLcdPrint(line1);

        const float displayTempC = getDisplayTemperature(lSens);
        const int centeredAngle = lSens.angleRawDeg - LAB3_1_ANGLE_OFFSET_DEG;
                const int tempInt = (int)displayTempC;
                int tempFrac = (int)((displayTempC - (float)tempInt) * 100.0f);
                if (tempFrac < 0) {
                        tempFrac = -tempFrac;
                }

                printf("Temp=%d.%02dC Angle=%d Off=%d Deb=%d Alert=%d Tv=%d\r\n",
                             tempInt,
                             tempFrac,
               lSens.angleRawDeg,
               centeredAngle,
               lSens.angleDebounceCounter,
               lSens.angleAlertState,
               lSens.temperatureValid ? 1 : 0);
    }
}
