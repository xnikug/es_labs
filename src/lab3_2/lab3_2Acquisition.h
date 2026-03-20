/**
 * @file lab3_2Acquisition.h
 * @brief Acquisition interfaces for Lab 3.2.
 */

#ifndef LAB3_2_ACQUISITION_H
#define LAB3_2_ACQUISITION_H

#include <Arduino_FreeRTOS.h>

void lab3_2AcquisitionInit();
void vTask3_2Acquisition(void *pvParameters);
void vTask3_2TempAcquisition(void *pvParameters);

#endif
