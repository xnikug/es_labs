#ifndef LAB3_1_SHARED_H
#define LAB3_1_SHARED_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Angle alert configuration
#define LAB3_1_ANGLE_OFFSET_DEG 135
#define LAB3_1_ANGLE_LOW_THRESHOLD_DEG -80
#define LAB3_1_ANGLE_HIGH_THRESHOLD_DEG 80
#define LAB3_1_ANGLE_DEBOUNCE_MAX_COUNT 5

// Task configuration
#define TASK_ACQUISITION_PERIOD_MS 100
#define TASK_CONDITIONING_PERIOD_MS 1000
#define TASK_ALERT_PERIOD_MS 100
#define TASK_DISPLAY_PERIOD_MS 1000

typedef enum {
    LAB3_1_ALERT_NORMAL = 0,
    LAB3_1_ALERT_ACTIVE = 1
} Lab3_1AlertState;

typedef struct {
    int      angleRawDeg;
    int      angleFilteredDeg;
    int      angleDebounceCounter;
    float    temperatureC;
    bool     temperatureValid;
    int      angleAlertState;
    int      angleDegAtAlertDecision;
    uint32_t readingCount;
} SensorReadings_t;

// Extern Globals
extern SensorReadings_t g_sensorData;
extern SemaphoreHandle_t xNewReadingSemaphore;
extern SemaphoreHandle_t xSensorMutex;

void lab3_1SharedInit();

#endif
