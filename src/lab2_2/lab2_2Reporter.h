/**
 * @file lab2_2Reporter.h
 * @brief Lab 2.2 - T3 Periodic reporter task (interface).
 *
 * Fires every 10 s via xTaskDelayUntil, prints total/short/long press counts
 * and average press duration to STDIO, then resets all statistics counters.
 */

#ifndef LAB2_2_REPORTER_H
#define LAB2_2_REPORTER_H

/**
 * @brief FreeRTOS task function for T3 (periodic reporter).
 * @param pvParameters Unused; pass NULL when creating the task.
 */
void vTaskReporter(void *pvParameters);

#endif // LAB2_2_REPORTER_H
