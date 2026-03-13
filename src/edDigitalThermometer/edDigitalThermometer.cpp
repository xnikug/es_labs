#include "edDigitalThermometer.h"
#include <OneWire.h>
#include <DallasTemperature.h>

static float gTemperatureC = 0.0f;
static SemaphoreHandle_t gThermoMutex = NULL;

static OneWire gOneWire(ED_DIGITAL_THERMOMETER_PIN);
static DallasTemperature gTemperatureSensors(&gOneWire);

void edDigitalThermometerSetup()
{
    gTemperatureSensors.begin();
    gTemperatureSensors.setWaitForConversion(true);
    gTemperatureSensors.setResolution(12);
    gThermoMutex = xSemaphoreCreateMutex();
}

SemaphoreHandle_t edDigitalThermometerGetMutex()
{
    return gThermoMutex;
}

void edDigitalThermometerLoop()
{
    gTemperatureSensors.requestTemperatures();
    const float measuredC = gTemperatureSensors.getTempCByIndex(0);

    if (gThermoMutex != NULL) {
        xSemaphoreTake(gThermoMutex, portMAX_DELAY);
        if (measuredC != DEVICE_DISCONNECTED_C) {
            gTemperatureC = measuredC;
        }
        xSemaphoreGive(gThermoMutex);
        return;
    }

    if (measuredC != DEVICE_DISCONNECTED_C) {
        gTemperatureC = measuredC;
    }
}

float edDigitalThermometerGetTemperatureC()
{
    float value = gTemperatureC;
    if (gThermoMutex != NULL) {
        xSemaphoreTake(gThermoMutex, portMAX_DELAY);
        value = gTemperatureC;
        xSemaphoreGive(gThermoMutex);
    }
    return value;
}

void edDigitalThermometerSetTemperatureC(float temperatureC)
{
    if (gThermoMutex != NULL) {
        xSemaphoreTake(gThermoMutex, portMAX_DELAY);
        gTemperatureC = temperatureC;
        xSemaphoreGive(gThermoMutex);
        return;
    }

    gTemperatureC = temperatureC;
}
