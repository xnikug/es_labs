/**
 * @file lab3_2Display.cpp
 * @brief LCD and serial reporting for Lab 3.2.
 */

#include "lab3_2Display.h"
#include "lab3_2Shared.h"
#include "../ddLcd/ddLcd.h"
#include <stdio.h>

static void splitTemp(float valueC, int *intPart, int *fracPart)
{
    *intPart = (int)valueC;
    int frac = (int)((valueC - (float)(*intPart)) * 100.0f);
    if (frac < 0) {
        frac = -frac;
    }
    *fracPart = frac;
}

void lab3_2DisplayInit()
{
    ddLcdInit();
}

void vTask3_2Display(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(1000));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_2_REPORT_PERIOD_MS);

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        Lab3_2Readings_t snapshot;
        if (xSemaphoreTake(xLab3_2SensorMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
            continue;
        }
        snapshot = gLab3_2Data;
        xSemaphoreGive(xLab3_2SensorMutex);

        char line0[17];
        char line1[17];

        int tempRawI = 0;
        int tempRawF = 0;
        int tempFiltI = 0;
        int tempFiltF = 0;
        splitTemp(snapshot.rawTempC, &tempRawI, &tempRawF);
        splitTemp(snapshot.tempAfterWeightedMeanC, &tempFiltI, &tempFiltF);

        snprintf(line0, sizeof(line0), "AR:%3d AF:%3d", snapshot.rawAdc, snapshot.adcAfterWeightedMean);
        snprintf(line1, sizeof(line1), "TR:%2d.%02d TF:%2d.%02d", tempRawI, tempRawF, tempFiltI, tempFiltF);

        ddLcdClear();
        ddLcdSetCursor(0, 0);
        ddLcdPrint(line0);
        ddLcdSetCursor(0, 1);
        ddLcdPrint(line1);

         printf("[L3.2] ADCraw=%d ADCsp=%d ADCw=%d U=%dmV Angle=%ddeg Cnt=%lu "
             "Traw=%d.%02dC Tsat=%d.%02dC Tsp=%d.%02dC Tw=%d.%02dC Tcnt=%lu\r\n",
               snapshot.rawAdc,
               snapshot.adcAfterSaltPepper,
               snapshot.adcAfterWeightedMean,
               snapshot.voltageMv,
               snapshot.angleDeg,
             (unsigned long)snapshot.readingCount,
             tempRawI,
             tempRawF,
             (int)snapshot.tempAfterSaturationC,
             (int)((snapshot.tempAfterSaturationC - (int)snapshot.tempAfterSaturationC) * 100.0f),
             (int)snapshot.tempAfterSaltPepperC,
             (int)((snapshot.tempAfterSaltPepperC - (int)snapshot.tempAfterSaltPepperC) * 100.0f),
             tempFiltI,
             tempFiltF,
             (unsigned long)snapshot.tempReadingCount);
    }
}
