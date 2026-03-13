#ifndef LAB3_1_ACQUISITION_H
#define LAB3_1_ACQUISITION_H

#include <Arduino_FreeRTOS.h>

void lab3_1AcquisitionInit();
void vTaskAngleAcquisition(void *pvParameters);
void vTaskTempAcquisition(void *pvParameters);

#endif
