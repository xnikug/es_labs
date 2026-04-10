#include "lab4_2Report.h"

#include "lab4_2Shared.h"
#include "../ddLcd/ddLcd.h"
#include <stdio.h>

static const char *lab4_2StateCode(lab4_2MotorHealth_t state)
{
    switch (state) {
    case LAB4_2_MOTOR_IDLE:
        return "IDL";
    case LAB4_2_MOTOR_ACTIVE:
        return "RUN";
    case LAB4_2_MOTOR_HOT:
        return "HOT";
    default:
        return "UNK";
    }
}

void vTask4_2Report(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelay(pdMS_TO_TICKS(LAB4_2_TASK_OFFSET_MS));

    for (;;) {
        const int target = gLab4_2TargetPower;
        const int actual = gLab4_2AppliedPower;

        if (gLab4_2OutputMode == 0) {
            const char inputTag = 'U';
            printf("motor|set=%+04d|out=%+04d|st=%s|hot=%02d/%02d|in=%c\n",
                   target,
                   actual,
                   lab4_2StateCode(gLab4_2Health),
                   gLab4_2HotCounter,
                   LAB4_2_HOT_COUNTER_LIMIT,
                   inputTag);
        } else {
            char line0[17];
            char line1[17];

            const char inputTag = 'U';
            snprintf(line0, sizeof(line0), "S%+04d O%+04d %c", target, actual, inputTag);
            snprintf(line1, sizeof(line1), "%s H%02d/%02d", lab4_2StateCode(gLab4_2Health), gLab4_2HotCounter, LAB4_2_HOT_COUNTER_LIMIT);

            ddLcdSetCursor(0, 0);
            ddLcdPrint("                ");
            ddLcdSetCursor(0, 0);
            ddLcdPrint(line0);

            ddLcdSetCursor(0, 1);
            ddLcdPrint("                ");
            ddLcdSetCursor(0, 1);
            ddLcdPrint(line1);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB4_2_REPORT_PERIOD_MS));
    }
}
