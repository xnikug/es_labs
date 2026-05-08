#include "lab5_1Acquisition.h"
#include "lab5_1Shared.h"
#include "../drivers/dht22_stdio/dht22_stdio.h"

#include <Arduino_FreeRTOS.h>

void lab5_1AcquisitionInit(void)
{
    dht22_stdio_init(APP_DHT22_PIN);
}

void vTask5_1Acquisition(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        SensorData_t data;
        data.timestampMs = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
        data.valid = dht22_read(&data.temperatureC, &data.humidityPct);
        appSharedSetSensor(&data);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_1_TASK_ACQ_MS));
    }
}
