#ifndef DD_SNS_ANGLE_H
#define DD_SNS_ANGLE_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define DD_SNS_ANGLE_MIN_DEG 0
#define DD_SNS_ANGLE_MAX_DEG 270

void ddSnsAngleSetup();
int ddSnsAngleGetValue();
void ddSnsAngleSetValue(int angleDeg);
SemaphoreHandle_t ddSnsAngleGetMutex();

#endif
