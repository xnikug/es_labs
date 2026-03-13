#include "lab3_1Acquisition.h"
#include "lab3_1Shared.h"
#include "../edPotentiometer/edPotentiometer.h"
#include "../ddSnsAngle/ddSnsAngle.h"
#include "../edDigitalThermometer/edDigitalThermometer.h"
#include <Arduino.h>

static bool isTemperatureValid(float temperatureC)
{
    if (isnan(temperatureC)) {
        return false;
    }

    return (temperatureC >= -55.0f) && (temperatureC <= 125.0f);
}

void lab3_1AcquisitionInit() {
    edPotentiometerSetup();
    ddSnsAngleSetup();
    edDigitalThermometerSetup();
}

void vTaskAngleAcquisition(void *pvParameters) {
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_ACQUISITION_PERIOD_MS);

    for (;;) {
        edPotentiometerLoop();

        const int potMilliVolts = edPotentiometerGetMilliVolts();
        const int measuredAngleDeg = map(
            potMilliVolts,
            ED_POTENTIOMETER_VOLTAGE_MIN_MV,
            ED_POTENTIOMETER_VOLTAGE_MAX_MV,
            DD_SNS_ANGLE_MIN_DEG,
            DD_SNS_ANGLE_MAX_DEG
        );
        ddSnsAngleSetValue(measuredAngleDeg);

        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            g_sensorData.angleRawDeg = measuredAngleDeg;
            g_sensorData.readingCount++;
            xSemaphoreGive(xSensorMutex);
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void vTaskTempAcquisition(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);
    uint8_t printDivider = 0;

    for (;;) {
        edDigitalThermometerLoop();

        const float measuredTempC = edDigitalThermometerGetTemperatureC();
        const bool validSample = isTemperatureValid(measuredTempC);

        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            if (validSample) {
                g_sensorData.temperatureC = measuredTempC;
            }
            g_sensorData.temperatureValid = validSample;
            xSemaphoreGive(xSensorMutex);
        }

        if (++printDivider >= 10) {
            printDivider = 0;
            const int whole = (int)measuredTempC;
            int frac = (int)((measuredTempC - (float)whole) * 100.0f);
            if (frac < 0) {
                frac = -frac;
            }
            printf("[TempAcq] raw=%d.%02dC valid=%d\r\n", whole, frac, validSample ? 1 : 0);
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
