#include "lab4_2SerialInput.h"

#include "lab4_2Shared.h"
#include "../srvSerialStdio/srvSerialStdio.h"

#include <stdlib.h>
#include <stdio.h>

static bool lab4_2TryParseToken(const char *token, int *value)
{
    if (token == NULL || value == NULL) {
        return false;
    }

    char *endPtr = NULL;
    long parsed = strtol(token, &endPtr, 10);

    if (endPtr == token) {
        return false;
    }

    if (*endPtr != '\0') {
        return false;
    }

    if (parsed < LAB4_2_POWER_MIN || parsed > LAB4_2_POWER_MAX) {
        return false;
    }

    *value = (int)parsed;
    return true;
}

void vTask4_2SerialInput(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(2000));

    char commandToken[24];

    for (;;) {
        int raw = gLab4_2LastSerialValue;

        if (srvSerialAvailable() > 0) {
            int scanned = scanf("%23s", commandToken);
            if (scanned == 1) {
                int parsed = 0;
                if (lab4_2TryParseToken(commandToken, &parsed)) {
                    raw = parsed;
                    gLab4_2LastSerialValue = parsed;
                    printf("ACK|set_raw=%d\n", parsed);
                } else {
                    printf("ERR|cmd=invalid_or_oob|expected=int[-100..100]\n");
                }
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
