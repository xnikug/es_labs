#include "lab3_1Shared.h"

SensorReadings_t g_sensorData = {0, 0, 0, 0.0f, false, LAB3_1_ALERT_NORMAL, 0, 0};

SemaphoreHandle_t xNewReadingSemaphore = NULL;
SemaphoreHandle_t xSensorMutex = NULL;

void lab3_1SharedInit() {
    xNewReadingSemaphore = xSemaphoreCreateBinary();
    xSensorMutex = xSemaphoreCreateMutex();
}
