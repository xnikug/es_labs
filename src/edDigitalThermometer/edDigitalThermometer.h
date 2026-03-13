#ifndef ED_DIGITAL_THERMOMETER_H
#define ED_DIGITAL_THERMOMETER_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define ED_DIGITAL_THERMOMETER_PIN 5

void edDigitalThermometerSetup();
void edDigitalThermometerLoop();
float edDigitalThermometerGetTemperatureC();
void edDigitalThermometerSetTemperatureC(float temperatureC);
SemaphoreHandle_t edDigitalThermometerGetMutex();

#endif
