#include "lab5_1Control.h"
#include "lab5_1Shared.h"
#include "../control/onoff_control.h"

#include <Arduino_FreeRTOS.h>

void vTask5_1Control(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        sensor_data_t   sensor;
        control_state_t ctrl;
        app_shared_get_sensor(&sensor);
        app_shared_get_control(&ctrl);

        if (sensor.valid) {
            relay_cmd_t out = onoff_control_update(
                sensor.temperature_c,
                ctrl.setpoint_c,
                ctrl.hysteresis_c,
                ctrl.onoff_output);
            if (out != ctrl.onoff_output)
                app_shared_set_onoff_output(out,
                    (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_1_TASK_CTRL_MS));
    }
}
