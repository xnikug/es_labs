/**
 * @file lab2_2App.h
 * @brief Application interface for Lab 2.2 – FreeRTOS Button Statistics.
 *
 * Runs 3 concurrent FreeRTOS tasks on an Arduino Mega 2560:
 *
 * | Task | Function         | Period       | Sync              | Role                                   |
 * |------|------------------|--------------|-------------------|----------------------------------------|
 * | T1   | vTaskButtonLed   | 10 ms poll   | –                 | Measures press duration, lights LED    |
 * | T2   | vTaskStatsYellow | event-driven | xSemaphoreTake    | Updates statistics, blinks yellow LED  |
 * | T3   | vTaskReport      | 10 s         | –                 | Prints stats to STDIO, resets counters |
 *
 * Hardware (Arduino Mega 2560):
 *   PIN  3 – pushbutton (INPUT_PULLUP, active LOW)
 *   PIN 12 – green  LED (T1: short press indicator, ~1 s)
 *   PIN 10 – red    LED (T1: long  press indicator, ~1 s)
 *   PIN 11 – yellow LED (T2: blink 5× short / 10× long)
 *
 * Synchronisation:
 *   xButtonSemaphore – binary semaphore, T1 gives after LED expires, T2 takes
 *   xMutexDuration   – guards last_press_duration (T1 writes, T2 reads)
 *   xMutexStats      – guards statistics counters  (T2 writes, T3 reads/resets)
 *
 * stdout → UART via srvSerialStdio (9600 baud).
 */

#ifndef LAB2_2_APP_H
#define LAB2_2_APP_H

/**
 * @brief Initialises peripherals, serial STDIO, FreeRTOS objects, and tasks.
 *        The FreeRTOS scheduler starts automatically after setup() returns.
 */
void lab2_2AppSetup();

/**
 * @brief Empty – the FreeRTOS scheduler owns the CPU after setup().
 */
void lab2_2AppLoop();

#endif // LAB2_2_APP_H
