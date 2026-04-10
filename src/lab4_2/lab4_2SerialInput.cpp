#include "lab4_2SerialInput.h"

#include "lab4_2Shared.h"
#include "../srvSerialStdio/srvSerialStdio.h"

void vTask4_2SerialInput(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(2000));

    for (;;) {
        int raw = gLab4_2LastSerialValue;

        if (srvSerialAvailable() > 0) {
            int parsed = 0;
            if (scanf("%d", &parsed) == 1) {
                raw = parsed;
                gLab4_2LastSerialValue = parsed;
            }
        }

        gLab4_2SerialRaw = raw;

        const int sat = lab4_2ClampInt(raw, LAB4_2_POWER_MIN, LAB4_2_POWER_MAX);
        gLab4_2SerialSat = sat;

        const int med = lab4_2MedianFilter5(sat);
        gLab4_2SerialMedian = med;

        const int weighted = lab4_2WeightedFilter5(med);
        gLab4_2SerialWeighted = weighted;

        if (xSemaphoreTake(gLab4_2PowerMutex, portMAX_DELAY) == pdTRUE) {
            gLab4_2SerialControlPower = weighted;
            xSemaphoreGive(gLab4_2PowerMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(LAB4_2_INPUT_SCAN_PERIOD_MS));
    }
}
