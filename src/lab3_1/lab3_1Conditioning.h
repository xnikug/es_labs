/**
 * @file lab3_1Conditioning.h
 * @brief Conditioning and alert task interfaces for Lab 3.1.
 */

#ifndef LAB3_1_CONDITIONING_H
#define LAB3_1_CONDITIONING_H

#include <Arduino_FreeRTOS.h>

/**
 * @brief Periodic task that applies angle offset and debounce processing.
 */
void vTaskConditioning(void *pvParameters);

/**
 * @brief Periodic task that derives final alert state from debounce output.
 */
void vTaskAngleAlert(void *pvParameters);

#endif
