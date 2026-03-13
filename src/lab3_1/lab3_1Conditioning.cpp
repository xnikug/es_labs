/**
 * @file lab3_1Conditioning.cpp
 * @brief Angle conditioning and alert decision tasks.
 */

#include "lab3_1Conditioning.h"
#include "lab3_1Shared.h"
#include "../ddSnsAngle/ddSnsAngle.h"

/**
 * @brief Periodic conditioning task.
 *
 * Reads raw angle, applies centered threshold check and updates
 * a saturating debounce counter.
 */
void vTaskConditioning(void *pvParameters)
{
    (void)pvParameters;

    // Startup offset to avoid overlap with first acquisition cycles.
    vTaskDelay(pdMS_TO_TICKS(600));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_CONDITIONING_PERIOD_MS);

    int debounceCounter = 0;

    for (;;) {
        // Acquire latest raw angle produced by acquisition stage.
        const int rawAngleDeg = ddSnsAngleGetValue();

        const int centeredAngleDeg = rawAngleDeg - LAB3_1_ANGLE_OFFSET_DEG;
        const bool outsideWindow =
            (centeredAngleDeg < LAB3_1_ANGLE_LOW_THRESHOLD_DEG) ||
            (centeredAngleDeg > LAB3_1_ANGLE_HIGH_THRESHOLD_DEG);

        // Saturating up/down debounce counter behavior.
        if (outsideWindow) {
            if (debounceCounter < LAB3_1_ANGLE_DEBOUNCE_MAX_COUNT) {
                debounceCounter++;
            }
        } else if (debounceCounter > 0) {
            debounceCounter--;
        }

        // Publish conditioned values for downstream tasks.
        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            g_sensorData.angleRawDeg = rawAngleDeg;
            g_sensorData.angleFilteredDeg = rawAngleDeg;
            g_sensorData.angleDebounceCounter = debounceCounter;
            xSemaphoreGive(xSensorMutex);
        }

        // Keep strict periodic cadence.
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * @brief Periodic alert task.
 *
 * Computes binary alert state from conditioned debounce counter and
 * stores angle snapshot associated with the decision.
 */
void vTaskAngleAlert(void *pvParameters)
{
    (void)pvParameters;

    // Startup offset to let conditioning stage produce first values.
    vTaskDelay(pdMS_TO_TICKS(700));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_ALERT_PERIOD_MS);

    for (;;) {
        int rawAngleDeg = 0;
        int debounceCounter = 0;

        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            rawAngleDeg = g_sensorData.angleRawDeg;
            debounceCounter = g_sensorData.angleDebounceCounter;
            xSemaphoreGive(xSensorMutex);
        }

        // Alert activates only after debounce reaches configured threshold.
        const int alertState = (debounceCounter >= LAB3_1_ANGLE_DEBOUNCE_MAX_COUNT) ? LAB3_1_ALERT_ACTIVE : LAB3_1_ALERT_NORMAL;

        if (xSemaphoreTake(xSensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            g_sensorData.angleAlertState = alertState;
            g_sensorData.angleDegAtAlertDecision = rawAngleDeg;
            xSemaphoreGive(xSensorMutex);
        }

        // Keep strict periodic cadence.
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
