/**
 * @file lab3_2Display.h
 * @brief Display interfaces for Lab 3.2.
 */

#ifndef LAB3_2_DISPLAY_H
#define LAB3_2_DISPLAY_H

#include <Arduino_FreeRTOS.h>

extern uint8_t gLab3_2ReportMode;

void lab3_2DisplayInit();
void vTask3_2Display(void *pvParameters);

#endif
