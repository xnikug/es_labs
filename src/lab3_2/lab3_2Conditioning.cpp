/**
 * @file lab3_2Conditioning.cpp
 * @brief Signal conditioning pipeline for Lab 3.2.
 */

#include "lab3_2Conditioning.h"
#include "lab3_2Shared.h"
#include "../edPotentiometer/edPotentiometer.h"
#include <math.h>

static int saturateInt(int value, int minValue, int maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static int applySaltAndPepperFilter(int prev2, int prev1, int sample)
{
    const int diffToPrev1 = abs(sample - prev1);
    const int diffToPrev2 = abs(sample - prev2);
    const int baseDiff = abs(prev1 - prev2);

    if ((diffToPrev1 > LAB3_2_ADC_SALT_PEPPER_THRESHOLD) &&
        (diffToPrev2 > LAB3_2_ADC_SALT_PEPPER_THRESHOLD) &&
        (baseDiff < LAB3_2_ADC_SALT_PEPPER_THRESHOLD)) {
        return (prev1 + prev2) / 2;
    }

    return sample;
}

static int applyWeightedMeanFilter(int prev2, int prev1, int current)
{
    return (prev2 + (2 * prev1) + (3 * current)) / 6;
}

static float saturateFloat(float value, float minValue, float maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static float applySaltAndPepperFilterFloat(float prev2, float prev1, float sample)
{
    const float diffToPrev1 = fabsf(sample - prev1);
    const float diffToPrev2 = fabsf(sample - prev2);
    const float baseDiff = fabsf(prev1 - prev2);

    if ((diffToPrev1 > LAB3_2_TEMP_SALT_PEPPER_THRESHOLD_C) &&
        (diffToPrev2 > LAB3_2_TEMP_SALT_PEPPER_THRESHOLD_C) &&
        (baseDiff < LAB3_2_TEMP_SALT_PEPPER_THRESHOLD_C)) {
        return (prev1 + prev2) * 0.5f;
    }

    return sample;
}

static float applyWeightedMeanFilterFloat(float prev2, float prev1, float current)
{
    return (prev2 + (2.0f * prev1) + (3.0f * current)) / 6.0f;
}

void vTask3_2Conditioning(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(700));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_2_CONDITIONING_PERIOD_MS);

    int rawPrev2 = 0;
    int rawPrev1 = 0;
    int spPrev2 = 0;
    int spPrev1 = 0;
    bool initialized = false;

    for (;;) {
        int rawAdc = 0;

        if (xSemaphoreTake(xLab3_2SensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            rawAdc = gLab3_2Data.rawAdc;
            xSemaphoreGive(xLab3_2SensorMutex);
        }

        rawAdc = saturateInt(rawAdc, ED_POTENTIOMETER_ADC_MIN, ED_POTENTIOMETER_ADC_MAX);

        if (!initialized) {
            rawPrev2 = rawAdc;
            rawPrev1 = rawAdc;
            spPrev2 = rawAdc;
            spPrev1 = rawAdc;
            initialized = true;
        }

        const int filteredSaltPepper = applySaltAndPepperFilter(rawPrev2, rawPrev1, rawAdc);
        const int filteredWeighted = applyWeightedMeanFilter(spPrev2, spPrev1, filteredSaltPepper);

        const int saturatedAdc = saturateInt(filteredWeighted, ED_POTENTIOMETER_ADC_MIN, ED_POTENTIOMETER_ADC_MAX);
        const int voltageMv = map(
            saturatedAdc,
            ED_POTENTIOMETER_ADC_MIN,
            ED_POTENTIOMETER_ADC_MAX,
            ED_POTENTIOMETER_VOLTAGE_MIN_MV,
            ED_POTENTIOMETER_VOLTAGE_MAX_MV
        );
        const int saturatedVoltageMv = saturateInt(voltageMv, ED_POTENTIOMETER_VOLTAGE_MIN_MV, ED_POTENTIOMETER_VOLTAGE_MAX_MV);

        const int angleDeg = map(
            saturatedVoltageMv,
            ED_POTENTIOMETER_VOLTAGE_MIN_MV,
            ED_POTENTIOMETER_VOLTAGE_MAX_MV,
            LAB3_2_ANGLE_MIN_DEG,
            LAB3_2_ANGLE_MAX_DEG
        );
        const int saturatedAngleDeg = saturateInt(angleDeg, LAB3_2_ANGLE_MIN_DEG, LAB3_2_ANGLE_MAX_DEG);

        if (xSemaphoreTake(xLab3_2SensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            gLab3_2Data.adcAfterSaltPepper = filteredSaltPepper;
            gLab3_2Data.adcAfterWeightedMean = saturatedAdc;
            gLab3_2Data.voltageMv = saturatedVoltageMv;
            gLab3_2Data.angleDeg = saturatedAngleDeg;
            xSemaphoreGive(xLab3_2SensorMutex);
        }

        rawPrev2 = rawPrev1;
        rawPrev1 = rawAdc;
        spPrev2 = spPrev1;
        spPrev1 = filteredSaltPepper;

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void vTask3_2TempConditioning(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(900));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_2_TEMP_CONDITIONING_PERIOD_MS);

    float satPrev2 = LAB3_2_TEMP_SAT_MIN_C;
    float satPrev1 = LAB3_2_TEMP_SAT_MIN_C;
    float spPrev2 = LAB3_2_TEMP_SAT_MIN_C;
    float spPrev1 = LAB3_2_TEMP_SAT_MIN_C;
    bool initialized = false;

    for (;;) {
        float rawTempC = LAB3_2_TEMP_SAT_MIN_C;

        if (xSemaphoreTake(xLab3_2SensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            rawTempC = gLab3_2Data.rawTempC;
            xSemaphoreGive(xLab3_2SensorMutex);
        }

        const float saturatedTemp = saturateFloat(rawTempC, LAB3_2_TEMP_SAT_MIN_C, LAB3_2_TEMP_SAT_MAX_C);

        if (!initialized) {
            satPrev2 = saturatedTemp;
            satPrev1 = saturatedTemp;
            spPrev2 = saturatedTemp;
            spPrev1 = saturatedTemp;
            initialized = true;
        }

        const float filteredSaltPepper = applySaltAndPepperFilterFloat(satPrev2, satPrev1, saturatedTemp);
        const float filteredWeighted = applyWeightedMeanFilterFloat(spPrev2, spPrev1, filteredSaltPepper);
        const float finalTemp = saturateFloat(filteredWeighted, LAB3_2_TEMP_SAT_MIN_C, LAB3_2_TEMP_SAT_MAX_C);

        if (xSemaphoreTake(xLab3_2SensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            gLab3_2Data.tempAfterSaturationC = saturatedTemp;
            gLab3_2Data.tempAfterSaltPepperC = filteredSaltPepper;
            gLab3_2Data.tempAfterWeightedMeanC = finalTemp;
            xSemaphoreGive(xLab3_2SensorMutex);
        }

        satPrev2 = satPrev1;
        satPrev1 = saturatedTemp;
        spPrev2 = spPrev1;
        spPrev1 = filteredSaltPepper;

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
