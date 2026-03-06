/**
 * @file lab2_2ButtonMonitor.h
 * @brief Lab 2.2 - T1 Button monitor task (interface).
 *
 * Polls the button every POLL_TICKS (1 tick = ~16 ms), measures press duration
 * using a 2-state machine, lights the green LED for short presses (<500 ms) or
 * the red LED for long presses (>=500 ms) for ~1 s, then signals T2 via
 * xPressSemaphore once the LED turns off.
 */

#ifndef LAB2_2_BUTTON_MONITOR_H
#define LAB2_2_BUTTON_MONITOR_H

/**
 * @brief FreeRTOS task function for T1 (button monitor).
 * @param pvParameters Unused; pass NULL when creating the task.
 */
void vTaskButtonMonitor(void *pvParameters);

#endif // LAB2_2_BUTTON_MONITOR_H
