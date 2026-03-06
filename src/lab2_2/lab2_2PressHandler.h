/**
 * @file lab2_2PressHandler.h
 * @brief Lab 2.2 - T2 Press handler task (interface).
 *
 * Blocks on xPressSemaphore (given by T1), updates press-duration statistics,
 * then blinks the yellow LED 5x for a short press or 10x for a long press
 * at 100 ms ON / 100 ms OFF per blink.
 */

#ifndef LAB2_2_PRESS_HANDLER_H
#define LAB2_2_PRESS_HANDLER_H

/**
 * @brief FreeRTOS task function for T2 (press handler & yellow blink).
 * @param pvParameters Unused; pass NULL when creating the task.
 */
void vTaskPressHandler(void *pvParameters);

#endif // LAB2_2_PRESS_HANDLER_H
