/**
 * @file lab2_2App.cpp
 * @brief Lab 2.2 - FreeRTOS Button Statistics (Arduino Mega 2560).
 *
 * Owns all shared state (semaphore, mutexes, variables) and exposes them
 * via the getter/setter API declared in lab2_2App.h. The three task files
 * include lab2_2App.h to access pins and shared state.
 *
 * Three concurrent FreeRTOS tasks:
 *
 *  T1 - vTaskButtonMonitor  (1 tick poll, xTaskDelayUntil)
 *       Polls button; on release computes press duration, lights the green
 *       LED for short presses (<500 ms) or the red LED for long presses
 *       (>=500 ms) using a tick countdown, then gives xPressSemaphore to T2.
 *
 *  T2 - vTaskPressHandler   (semaphore-driven)
 *       Takes xPressSemaphore, updates press-duration statistics, then
 *       blinks the yellow LED 5x (short) or 10x (long) at 100 ms ON / 100 ms OFF.
 *
 *  T3 - vTaskReporter       (10 s recurrence, xTaskDelayUntil)
 *       Prints total/short/long press counts and average duration to STDIO,
 *       then resets all counters.
 *
 * Hardware (Arduino Mega 2560):
 *   PIN  3 - pushbutton (INPUT_PULLUP, active LOW -> GND)
 *   PIN 12 - green  LED (T1: short press indicator, ~1 s)
 *   PIN 11 - red    LED (T1: long  press indicator, ~1 s)
 *   PIN 10 - yellow LED (T2: blink sequence)
 */

#include "lab2_2App.h"
#include "lab2_2ButtonMonitor.h"
#include "lab2_2PressHandler.h"
#include "lab2_2Reporter.h"
#include "srvSerialStdio/srvSerialStdio.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

// Synchronisation objects

SemaphoreHandle_t xPressSemaphore = NULL;

static SemaphoreHandle_t xDurationMutex = NULL;
static SemaphoreHandle_t xStatsMutex    = NULL;

// Shared variables

static int lastPressDuration = 0;
static int totalPresses      = 0;
static int shortPresses      = 0;
static int longPresses       = 0;
static int sumShortDurations = 0;
static int sumLongDurations  = 0;

// Duration getters / setters (xDurationMutex)

void setLastPressDuration(int value)
{
    xSemaphoreTake(xDurationMutex, portMAX_DELAY);
    lastPressDuration = value;
    xSemaphoreGive(xDurationMutex);
}

int getLastPressDuration()
{
    xSemaphoreTake(xDurationMutex, portMAX_DELAY);
    int val = lastPressDuration;
    xSemaphoreGive(xDurationMutex);
    return val;
}

// Statistics getters / setters (xStatsMutex)

void setTotalPresses(int value)
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    totalPresses = value;
    xSemaphoreGive(xStatsMutex);
}

int getTotalPresses()
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    int val = totalPresses;
    xSemaphoreGive(xStatsMutex);
    return val;
}

void setShortPresses(int value)
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    shortPresses = value;
    xSemaphoreGive(xStatsMutex);
}

int getShortPresses()
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    int val = shortPresses;
    xSemaphoreGive(xStatsMutex);
    return val;
}

void setLongPresses(int value)
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    longPresses = value;
    xSemaphoreGive(xStatsMutex);
}

int getLongPresses()
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    int val = longPresses;
    xSemaphoreGive(xStatsMutex);
    return val;
}

void setSumShortDurations(int value)
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    sumShortDurations = value;
    xSemaphoreGive(xStatsMutex);
}

int getSumShortDurations()
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    int val = sumShortDurations;
    xSemaphoreGive(xStatsMutex);
    return val;
}

void setSumLongDurations(int value)
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    sumLongDurations = value;
    xSemaphoreGive(xStatsMutex);
}

int getSumLongDurations()
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    int val = sumLongDurations;
    xSemaphoreGive(xStatsMutex);
    return val;
}

void resetStatistics()
{
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
    totalPresses      = 0;
    shortPresses      = 0;
    longPresses       = 0;
    sumShortDurations = 0;
    sumLongDurations  = 0;
    xSemaphoreGive(xStatsMutex);
}

// Setup helpers

static void initGpio()
{
    pinMode(PIN_BUTTON,     INPUT_PULLUP);
    pinMode(PIN_LED_GREEN,  OUTPUT);  digitalWrite(PIN_LED_GREEN,  LOW);
    pinMode(PIN_LED_RED,    OUTPUT);  digitalWrite(PIN_LED_RED,    LOW);
    pinMode(PIN_LED_YELLOW, OUTPUT);  digitalWrite(PIN_LED_YELLOW, LOW);
}

static void initSyncObjects()
{
    xPressSemaphore = xSemaphoreCreateBinary();
    xDurationMutex  = xSemaphoreCreateMutex();
    xStatsMutex     = xSemaphoreCreateMutex();

    if (xPressSemaphore == NULL || xDurationMutex == NULL || xStatsMutex == NULL) {
        printf("FATAL: Failed to create FreeRTOS objects!\n");
        for (;;);
    }
}

static void createTasks()
{
    xTaskCreate(vTaskButtonMonitor, "ButtonMonitor",  500, NULL, 3, NULL);
    xTaskCreate(vTaskPressHandler,  "PressHandler",   500, NULL, 2, NULL);
    xTaskCreate(vTaskReporter,      "Reporter",       500, NULL, 1, NULL);
}

void lab2_2AppSetup()
{
    srvSerialSetup(9600);

    printf("============================================\n");
    printf(" Lab 2.2 - FreeRTOS Button Statistics\n");
    printf("============================================\n");
    printf("HW: Arduino Mega 2560\n");
    printf("PIN_BUTTON=%d  LED_G=%d  LED_R=%d  LED_Y=%d\n",
           PIN_BUTTON, PIN_LED_GREEN, PIN_LED_RED, PIN_LED_YELLOW);
    printf("Tick period: %u ms\n", portTICK_PERIOD_MS);
    printf("Tasks:\n");
    printf("  T1 ButtonMonitor  1 tick poll     (xTaskDelayUntil, prio 3)\n");
    printf("  T2 PressHandler   event-driven    (xSemaphoreTake,  prio 2)\n");
    printf("  T3 Reporter       10 s period     (xTaskDelayUntil, prio 1)\n");
    printf("Press the button to start!\n\n");

    initGpio();
    initSyncObjects();
    createTasks();
}

void lab2_2AppLoop()
{
    // Intentionally empty - the FreeRTOS scheduler owns the CPU.
}
