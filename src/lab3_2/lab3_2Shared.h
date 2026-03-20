/**
 * @file lab3_2Shared.h
 * @brief Shared constants and synchronization objects for Lab 3.2.
 */

#ifndef LAB3_2_SHARED_H
#define LAB3_2_SHARED_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define LAB3_2_ADC_SALT_PEPPER_THRESHOLD 90
#define LAB3_2_TEMP_SALT_PEPPER_THRESHOLD_C 1.5f

#define TASK3_2_ACQUISITION_PERIOD_MS 100
#define TASK3_2_CONDITIONING_PERIOD_MS 200
#define TASK3_2_REPORT_PERIOD_MS 1000

#define TASK3_2_TEMP_ACQUISITION_PERIOD_MS 1000
#define TASK3_2_TEMP_CONDITIONING_PERIOD_MS 1000

#define LAB3_2_ANGLE_MIN_DEG 0
#define LAB3_2_ANGLE_MAX_DEG 360

#define LAB3_2_TEMP_SAT_MIN_C 21.0f
#define LAB3_2_TEMP_SAT_MAX_C 27.0f

typedef struct {
    int rawAdc;
    int adcAfterSaltPepper;
    int adcAfterWeightedMean;
    int voltageMv;
    int angleDeg;
    float rawTempC;
    float tempAfterSaturationC;
    float tempAfterSaltPepperC;
    float tempAfterWeightedMeanC;
    uint32_t readingCount;
    uint32_t tempReadingCount;
} Lab3_2Readings_t;

extern Lab3_2Readings_t gLab3_2Data;
extern SemaphoreHandle_t xLab3_2SensorMutex;

void lab3_2SharedInit();

#endif
