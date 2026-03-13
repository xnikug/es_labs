/**
 * @file lab3_1Acquisition.cpp
 * @brief Sensor acquisition tasks for Lab 3.1.
 */

#include "lab3_1Acquisition.h"
#include "lab3_1Shared.h"
#include "../edPotentiometer/edPotentiometer.h"
#include "../ddSnsAngle/ddSnsAngle.h"
#include "../edDigitalThermometer/edDigitalThermometer.h"
#include <Arduino.h>

/**
 * @brief Validates DS18B20 sample range.
 * @param temperatureC Temperature sample in Celsius.
 * @return true when the sample is finite and in valid DS18B20 range.
 */
static bool isTemperatureValid(float temperatureC)
{
    if (isnan(temperatureC)) {
        return false;
    }

    return (temperatureC >= -55.0f) && (temperatureC <= 125.0f);
}

/**
 * @brief Initializes all devices used by acquisition tasks.
 */
void lab3_1AcquisitionInit() {
    edPotentiometerSetup();
    ddSnsAngleSetup();
    edDigitalThermometerSetup();
}

/**
 * @brief FreeRTOS task for periodic angle acquisition from potentiometer.
 */
void vTaskAngleAcquisition(void *pvParameters) {
    (void)pvParameters;

    // Startup offset so all modules initialize before first sample.
    vTaskDelay(pdMS_TO_TICKS(500));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_ACQUISITION_PERIOD_MS);

    for (;;) {
        // Read potentiometer and convert from mV to angle domain.
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

        // Publish shared angle data with mutex protection.
        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            g_sensorData.angleRawDeg = measuredAngleDeg;
            g_sensorData.readingCount++;
            xSemaphoreGive(xSensorMutex);
        }

        // Keep strict periodic cadence.
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * @brief FreeRTOS task for periodic temperature acquisition from DS18B20.
 */
void vTaskTempAcquisition(void *pvParameters)
{
    (void)pvParameters;

    // Startup offset so task does not race initialization.
    vTaskDelay(pdMS_TO_TICKS(500));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);
    uint8_t printDivider = 0;

    for (;;) {
        edDigitalThermometerLoop();

        const float measuredTempC = edDigitalThermometerGetTemperatureC();
        const bool validSample = isTemperatureValid(measuredTempC);

        // Publish valid values and validity flag to shared state.
        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            if (validSample) {
                g_sensorData.temperatureC = measuredTempC;
            }
            g_sensorData.temperatureValid = validSample;
            xSemaphoreGive(xSensorMutex);
        }

        // Lightweight periodic diagnostic line for serial monitoring.
        if (++printDivider >= 10) {
            printDivider = 0;
            const int whole = (int)measuredTempC;
            int frac = (int)((measuredTempC - (float)whole) * 100.0f);
            if (frac < 0) {
                frac = -frac;
            }
            printf("[TempAcq] raw=%d.%02dC valid=%d\r\n", whole, frac, validSample ? 1 : 0);
        }

        // Keep strict periodic cadence.
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
