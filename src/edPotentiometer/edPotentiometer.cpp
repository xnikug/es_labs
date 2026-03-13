#include "edPotentiometer.h"
#include <Arduino.h>

static int gRawAdc = 0;
static int gMilliVolts = 0;
static SemaphoreHandle_t gPotMutex = NULL;

void edPotentiometerSetup()
{
    gPotMutex = xSemaphoreCreateMutex();
}

SemaphoreHandle_t edPotentiometerGetMutex()
{
    return gPotMutex;
}

void edPotentiometerLoop()
{
    const int measuredAdc = analogRead(ED_POTENTIOMETER_PIN);
    const int mappedMilliVolts = map(
        measuredAdc,
        ED_POTENTIOMETER_ADC_MIN,
        ED_POTENTIOMETER_ADC_MAX,
        ED_POTENTIOMETER_VOLTAGE_MIN_MV,
        ED_POTENTIOMETER_VOLTAGE_MAX_MV
    );

    if (gPotMutex != NULL) {
        xSemaphoreTake(gPotMutex, portMAX_DELAY);
        gRawAdc = measuredAdc;
        gMilliVolts = mappedMilliVolts;
        xSemaphoreGive(gPotMutex);
        return;
    }

    gRawAdc = measuredAdc;
    gMilliVolts = mappedMilliVolts;
}

int edPotentiometerGetRawAdc()
{
    int value = gRawAdc;
    if (gPotMutex != NULL) {
        xSemaphoreTake(gPotMutex, portMAX_DELAY);
        value = gRawAdc;
        xSemaphoreGive(gPotMutex);
    }
    return value;
}

int edPotentiometerGetMilliVolts()
{
    int value = gMilliVolts;
    if (gPotMutex != NULL) {
        xSemaphoreTake(gPotMutex, portMAX_DELAY);
        value = gMilliVolts;
        xSemaphoreGive(gPotMutex);
    }
    return value;
}
