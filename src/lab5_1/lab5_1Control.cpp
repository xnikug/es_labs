#include "lab5_1Control.h"
#include "lab5_1Shared.h"
#include "../control/onoffControl.h"

#include <Arduino_FreeRTOS.h>

void vTask5_1Control(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        SensorData_t   sensor;
        ControlState_t ctrl;
        appSharedGetSensor(&sensor);
        appSharedGetControl(&ctrl);

        if (sensor.valid) {
            RelayCmd_t out = onoffControlUpdate(
                sensor.temperatureC,
                ctrl.setpointC,
                ctrl.hysteresisC,
                ctrl.onoffOutput);
            if (out != ctrl.onoffOutput)
                appSharedSetOnoffOutput(out,
                    (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_1_TASK_CTRL_MS));
    }
}
