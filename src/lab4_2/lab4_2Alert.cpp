#include "lab4_2Alert.h"

#include "lab4_2Shared.h"

void vTask4_2Alert(void *pvParameters)
{
    (void)pvParameters;

    lab4_2AlertLedInit();
    bool blinkState = false;

    vTaskDelay(pdMS_TO_TICKS(LAB4_2_TASK_OFFSET_MS));

    for (;;) {
        const int speed = gLab4_2AppliedPower;
        const bool inAlertBand = (speed <= MOTOR_ALERT_LOW) || (speed >= MOTOR_ALERT_HIGH);

        if (inAlertBand) {
            blinkState = !blinkState;
            lab4_2AlertLedSet(blinkState);
        } else {
            blinkState = false;
            lab4_2AlertLedSet(false);
        }

        vTaskDelay(pdMS_TO_TICKS(LAB4_2_ALERT_PERIOD_MS));
    }
}
