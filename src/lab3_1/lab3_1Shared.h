/**
 * @file lab3_1Shared.h
 * @brief Shared types, constants and synchronization primitives for Lab 3.1.
 */

#ifndef LAB3_1_SHARED_H
#define LAB3_1_SHARED_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Angle alert configuration
// Raw angle domain is 0..270 deg, centered domain is -135..+135 deg.
#define LAB3_1_ANGLE_OFFSET_DEG 135
#define LAB3_1_ANGLE_LOW_THRESHOLD_DEG -80
#define LAB3_1_ANGLE_HIGH_THRESHOLD_DEG 80
#define LAB3_1_ANGLE_DEBOUNCE_MAX_COUNT 5

// Task periodic timing configuration (milliseconds)
#define TASK_ACQUISITION_PERIOD_MS 100
#define TASK_CONDITIONING_PERIOD_MS 1000
#define TASK_ALERT_PERIOD_MS 100
#define TASK_DISPLAY_PERIOD_MS 1000

/**
 * @brief Binary alert state for angle out-of-window detection.
 */
typedef enum {
    LAB3_1_ALERT_NORMAL = 0,
    LAB3_1_ALERT_ACTIVE = 1
} Lab3_1AlertState;

/**
 * @brief Shared data snapshot exchanged by tasks.
 */
typedef struct {
    /** Latest raw angle in degrees (0..270). */
    int      angleRawDeg;
    /** Conditioned angle (currently identical to raw angle). */
    int      angleFilteredDeg;
    /** Debounce counter used for angle alert stabilization. */
    int      angleDebounceCounter;
    /** Latest valid temperature in Celsius. */
    float    temperatureC;
    /** Flag indicating whether the latest sampled temperature is valid. */
    bool     temperatureValid;
    /** Current alert state (normal/active). */
    int      angleAlertState;
    /** Angle captured at alert decision time. */
    int      angleDegAtAlertDecision;
    /** Diagnostic total count of angle samples acquired. */
    uint32_t readingCount;
} SensorReadings_t;

// Extern Globals
extern SensorReadings_t g_sensorData;
extern SemaphoreHandle_t xNewReadingSemaphore;
extern SemaphoreHandle_t xSensorMutex;

/**
 * @brief Initializes shared synchronization objects.
 */
void lab3_1SharedInit();

#endif
