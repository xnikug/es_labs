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

#define REPORT_PERIOD_TICKS  (10000 / portTICK_PERIOD_MS)

/**
 * @brief Returns the average duration; 0 when count is zero.
 * @param sumDurations Total cumulative duration in ms.
 * @param count Number of presses.
 */
static int computeAverage(int sumDurations, int count)
{
    if (count > 0) {
        return sumDurations / count;
    }
    return 0;
}

/**
 * @brief Reads all statistics, prints them, and resets counters.
 */
static void printAndResetReport()
{
    int tp  = getTotalPresses();
    int sp  = getShortPresses();
    int lp  = getLongPresses();
    int ssd = getSumShortDurations();
    int sld = getSumLongDurations();

    int avgDuration = computeAverage(ssd + sld, tp);

    printf("\n========== TASK 3: PERIODIC REPORT (10s) ==========\n");
    printf("Total presses:      %d\n", tp);
    printf("Short presses:      %d (total duration: %d ms)\n", sp, ssd);
    printf("Long presses:       %d (total duration: %d ms)\n", lp, sld);
    printf("Average duration:   %d ms\n", avgDuration);
    printf("====================================================\n\n");

    resetStatistics();
}

void vTaskReporter(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    printf("[T3] Reporter started - period=10000 ms\n");

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, REPORT_PERIOD_TICKS);
        printAndResetReport();
    }
}
