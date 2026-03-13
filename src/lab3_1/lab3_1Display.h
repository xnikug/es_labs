/**
 * @file lab3_1Display.h
 * @brief LCD display task interfaces for Lab 3.1.
 */

#ifndef LAB3_1_DISPLAY_H
#define LAB3_1_DISPLAY_H

#include <Arduino_FreeRTOS.h>

/**
 * @brief Initializes LCD reporting module.
 */
void lab3_1DisplayInit();

/**
 * @brief Periodic reporting task for LCD and serial output.
 */
void vTaskDisplay(void *pvParameters);

#endif
