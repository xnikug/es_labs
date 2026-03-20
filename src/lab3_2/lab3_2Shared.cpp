/**
 * @file lab3_2Shared.cpp
 * @brief Shared state definitions for Lab 3.2.
 */

#include "lab3_2Shared.h"

Lab3_2Readings_t gLab3_2Data = {0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0};
SemaphoreHandle_t xLab3_2SensorMutex = NULL;

void lab3_2SharedInit()
{
    xLab3_2SensorMutex = xSemaphoreCreateMutex();
}
