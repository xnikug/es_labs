/**
 * @file lab2_2Reporter.cpp
 * @brief Lab 2.2 - T3 Periodic reporter task (implementation).
 *
 * Behaviour:
 *  - Fires every REPORT_PERIOD_TICKS (10 000 ms) using xTaskDelayUntil.
 *  - Reads all statistics counters (mutex-protected via getters in lab2_2App).
 *  - Prints total presses, short presses (with total duration),
 *    long presses (with total duration), and average press duration.
 *  - Resets all statistics counters after printing (mutex-protected).
 */

#include "lab2_2Reporter.h"
#include "lab2_2App.h"
#include <Arduino.h>
#include <stdio.h>

// Report period: 10 seconds
#define REPORT_PERIOD_TICKS  (10000 / portTICK_PERIOD_MS)

/* See lab2_2Reporter.h for documentation */
void vTaskReporter(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    printf("[T3] Reporter started - period=10000 ms\n");

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, REPORT_PERIOD_TICKS);

        // Read statistics (mutex-protected)
        int tp  = getTotalPresses();
        int sp  = getShortPresses();
        int lp  = getLongPresses();
        int ssd = getSumShortDurations();
        int sld = getSumLongDurations();

        // Calculate average duration
        int avg_duration = 0;
        if (tp > 0) {
            avg_duration = (ssd + sld) / tp;
        }

        printf("\n========== TASK 3: PERIODIC REPORT (10s) ==========\n");
        printf("Total presses:      %d\n", tp);
        printf("Short presses:      %d (total duration: %d ms)\n", sp, ssd);
        printf("Long presses:       %d (total duration: %d ms)\n", lp, sld);
        printf("Average duration:   %d ms\n", avg_duration);
        printf("====================================================\n\n");

        // Reset statistics (mutex-protected)
        resetStatistics();
    }
}
