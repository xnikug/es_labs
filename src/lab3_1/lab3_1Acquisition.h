/**
 * @file lab3_1Acquisition.h
 * @brief Acquisition stage interfaces for Lab 3.1.
 */

#ifndef LAB3_1_ACQUISITION_H
#define LAB3_1_ACQUISITION_H

#include <Arduino_FreeRTOS.h>

/**
 * @brief Initializes acquisition-related device modules.
 */
void lab3_1AcquisitionInit();

/**
 * @brief Periodic task that samples potentiometer and updates angle data.
 */
void vTaskAngleAcquisition(void *pvParameters);

/**
 * @brief Periodic task that samples DS18B20 and updates temperature data.
 */
void vTaskTempAcquisition(void *pvParameters);

#endif
