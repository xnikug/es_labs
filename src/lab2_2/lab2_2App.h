/**
 * @file lab2_2App.h
 * @brief Lab 2.2 - FreeRTOS Button Statistics, application entry point and shared state.
 *
 * Runs 3 concurrent FreeRTOS tasks on an Arduino Mega 2560:
 *
 * | Task | Function            | Period      | Sync           | Role                                  |
 * |------|---------------------|-------------|----------------|---------------------------------------|
 * | T1   | vTaskButtonMonitor  | 1 tick poll | -              | Measures press duration, lights LED   |
 * | T2   | vTaskPressHandler   | event-driven| xSemaphoreTake | Updates statistics, blinks yellow LED |
 * | T3   | vTaskReporter       | 10 s        | -              | Prints stats to STDIO, resets counters|
 *
 * Hardware (Arduino Mega 2560):
 *   PIN  3 - pushbutton (INPUT_PULLUP, active LOW)
 *   PIN 12 - green  LED (T1: short press indicator, ~1 s)
 *   PIN 10 - red    LED (T1: long  press indicator, ~1 s)
 *   PIN 11 - yellow LED (T2: blink 5x short / 10x long)
 *
 * Synchronisation:
 *   xPressSemaphore - binary semaphore, T1 gives after LED expires, T2 takes
 *   xDurationMutex  - guards last_press_duration (T1 writes, T2 reads)
 *   xStatsMutex     - guards statistics counters (T2 writes, T3 reads/resets)
 *
 * stdout - UART via srvSerialStdio (9600 baud).
 */

#ifndef LAB2_2_APP_H
#define LAB2_2_APP_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Pin definitions

#define PIN_BUTTON      3   /**< Pushbutton: INPUT_PULLUP, active LOW.            */
#define PIN_LED_GREEN   12  /**< T1: short press indicator, on for ~1 s.          */
#define PIN_LED_RED     11  /**< T1: long  press indicator, on for ~1 s.          */
#define PIN_LED_YELLOW  10  /**< T2: blink 5x (short press) or 10x (long press). */

// Synchronisation objects (defined in lab2_2App.cpp)

/** Binary semaphore: T1 gives after LED expires, T2 takes. */
extern SemaphoreHandle_t xPressSemaphore;

// Duration API (T1 writes, T2 reads) - protected by xDurationMutex

void setLastPressDuration(int value);
int  getLastPressDuration();

// Statistics API (T2 writes, T3 reads/resets) - protected by xStatsMutex

void setTotalPresses(int value);
int  getTotalPresses();

void setShortPresses(int value);
int  getShortPresses();

void setLongPresses(int value);
int  getLongPresses();

void setSumShortDurations(int value);
int  getSumShortDurations();

void setSumLongDurations(int value);
int  getSumLongDurations();

/** @brief Atomically resets all statistics counters to 0. */
void resetStatistics();

/**
 * @brief Initialises peripherals, serial STDIO, shared state, and FreeRTOS tasks.
 *        The FreeRTOS scheduler starts automatically after setup() returns.
 */
void lab2_2AppSetup();

/**
 * @brief Empty - the FreeRTOS scheduler owns the CPU after setup().
 */
void lab2_2AppLoop();

#endif // LAB2_2_APP_H
