#ifndef ED_POTENTIOMETER_H
#define ED_POTENTIOMETER_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define ED_POTENTIOMETER_PIN A0
#define ED_POTENTIOMETER_ADC_MIN 0
#define ED_POTENTIOMETER_ADC_MAX 1023
#define ED_POTENTIOMETER_VOLTAGE_MIN_MV 0
#define ED_POTENTIOMETER_VOLTAGE_MAX_MV 5000

void edPotentiometerSetup();
void edPotentiometerLoop();
int edPotentiometerGetRawAdc();
int edPotentiometerGetMilliVolts();
SemaphoreHandle_t edPotentiometerGetMutex();

#endif
