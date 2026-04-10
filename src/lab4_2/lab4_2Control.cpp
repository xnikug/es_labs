#include "lab4_2Control.h"

#include "lab4_2Shared.h"

void vTask4_2Control(void *pvParameters)
{
    (void)pvParameters;

    lab4_2MotorOutInit();
    lab4_2RelayInit();

    int currentPower = 0;
    int relayDebounceCounter = 0;
    bool relayState = false;

    vTaskDelay(pdMS_TO_TICKS(LAB4_2_TASK_OFFSET_MS));

    for (;;) {
        int requestedPower = 0;

        if (xSemaphoreTake(gLab4_2PowerMutex, portMAX_DELAY) == pdTRUE) {
            requestedPower = gLab4_2SerialControlPower;
            xSemaphoreGive(gLab4_2PowerMutex);
        }

        const bool wasStopped = (currentPower == 0);
        const bool startRequested = (requestedPower != 0);
        if (wasStopped && startRequested) {
            currentPower = (requestedPower > 0) ? LAB4_2_KICKSTART_POWER : -LAB4_2_KICKSTART_POWER;
            lab4_2MotorOutSetPercent(currentPower);
            vTaskDelay(pdMS_TO_TICKS(LAB4_2_KICKSTART_TIME_MS));
        }

        currentPower = lab4_2StepTowards(currentPower, requestedPower, LAB4_2_RAMP_DELTA_PER_CYCLE);
        const bool relayWanted = (currentPower != 0);
        if (relayWanted != relayState) {
            ++relayDebounceCounter;
            if (relayDebounceCounter >= RELAY_DEBOUNCE_CNT) {
                relayState = relayWanted;
                lab4_2RelaySet(relayState);
                relayDebounceCounter = 0;
            }
        } else {
            relayDebounceCounter = 0;
        }

        currentPower = lab4_2ClampInt(currentPower, LAB4_2_POWER_MIN, LAB4_2_POWER_MAX);
        lab4_2MotorOutSetPercent(currentPower);

        const int absPower = (currentPower < 0) ? -currentPower : currentPower;
        if (absPower == 0) {
            gLab4_2Health = LAB4_2_MOTOR_IDLE;
            gLab4_2HotCounter = 0;
        } else if (absPower > LAB4_2_HOT_THRESHOLD) {
            ++gLab4_2HotCounter;
            gLab4_2Health = (gLab4_2HotCounter >= LAB4_2_HOT_COUNTER_LIMIT) ? LAB4_2_MOTOR_HOT : LAB4_2_MOTOR_ACTIVE;
        } else {
            gLab4_2Health = LAB4_2_MOTOR_ACTIVE;
            gLab4_2HotCounter = 0;
        }

        gLab4_2TargetPower = requestedPower;
        gLab4_2AppliedPower = currentPower;

        vTaskDelay(pdMS_TO_TICKS(LAB4_2_CONTROL_PERIOD_MS));
    }
}
