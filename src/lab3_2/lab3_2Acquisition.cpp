/**
 * @file lab3_2Acquisition.cpp
 * @brief Raw potentiometer acquisition task for Lab 3.2.
 */

#include "lab3_2Acquisition.h"
#include "lab3_2Shared.h"
#include "../edPotentiometer/edPotentiometer.h"
#include "../edDigitalThermometer/edDigitalThermometer.h"

void lab3_2AcquisitionInit()
{
    edPotentiometerSetup();
    edDigitalThermometerSetup();
}

void vTask3_2Acquisition(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(400));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_2_ACQUISITION_PERIOD_MS);

    for (;;) {
        edPotentiometerLoop();
        const int rawAdc = edPotentiometerGetRawAdc();

        if (xSemaphoreTake(xLab3_2SensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            gLab3_2Data.rawAdc = rawAdc;
            gLab3_2Data.readingCount++;
            xSemaphoreGive(xLab3_2SensorMutex);
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void vTask3_2TempAcquisition(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_2_TEMP_ACQUISITION_PERIOD_MS);

    for (;;) {
        edDigitalThermometerLoop();
        const float rawTempC = edDigitalThermometerGetTemperatureC();

        if (xSemaphoreTake(xLab3_2SensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            gLab3_2Data.rawTempC = rawTempC;
            gLab3_2Data.tempReadingCount++;
            xSemaphoreGive(xLab3_2SensorMutex);
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
