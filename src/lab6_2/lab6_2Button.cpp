#include "lab6_2Button.h"
#include "lab6_2Shared.h"
#include "../app_config.h"
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

void vTask6_2Button(void *pvParams) {
    (void)pvParams;
    uint8_t    lastRaw   = HIGH;
    uint8_t    debounced = HIGH;
    uint8_t    prevDeb   = HIGH;
    TickType_t chgTime   = 0;

    for (;;) {
        vTaskDelay(1);   /* yield every tick (~16 ms) */

        uint8_t    raw = digitalRead(APP_LAB6_2_REQ_BTN_PIN);
        TickType_t now = xTaskGetTickCount();

        if (raw != lastRaw) { lastRaw = raw; chgTime = now; }

        if ((TickType_t)(now - chgTime) >= pdMS_TO_TICKS(APP_BTN_DEBOUNCE_MS)) {
            debounced = lastRaw;
        }

        if (prevDeb == HIGH && debounced == LOW) {
            xSemaphoreGive(g_northReqSem);
        }

        prevDeb = debounced;
    }
}
