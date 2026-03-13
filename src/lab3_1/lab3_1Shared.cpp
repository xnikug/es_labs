/**
 * @file lab3_1Shared.cpp
 * @brief Shared state and synchronization object definitions.
 */

#include "lab3_1Shared.h"

// Global shared state used by all Lab 3.1 tasks.
SensorReadings_t g_sensorData = {0, 0, 0, 0.0f, false, LAB3_1_ALERT_NORMAL, 0, 0};

// Synchronization objects for task-safe shared state access.
SemaphoreHandle_t xNewReadingSemaphore = NULL;
SemaphoreHandle_t xSensorMutex = NULL;

/**
 * @brief Creates synchronization primitives for Lab 3.1.
 */
void lab3_1SharedInit() {
    xNewReadingSemaphore = xSemaphoreCreateBinary();
    xSensorMutex = xSemaphoreCreateMutex();
}
