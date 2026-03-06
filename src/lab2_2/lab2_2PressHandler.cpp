/**
 * @file lab2_2PressHandler.cpp
 * @brief Lab 2.2 - T2 Press handler task (implementation).
 *
 * Behaviour:
 *  - Blocks indefinitely on xPressSemaphore (given by T1).
 *  - Reads the last press duration (mutex-protected via getLastPressDuration()).
 *  - Updates total / short / long press counters and cumulative durations
 *    (mutex-protected via setters in lab2_2App).
 *  - Blinks the yellow LED: SHORT_BLINKS (5) times for a short press,
 *    LONG_BLINKS (10) times for a long press; 100 ms ON / 100 ms OFF per blink.
 */

#include "lab2_2PressHandler.h"
#include "lab2_2App.h"
#include <Arduino.h>
#include <stdio.h>

#define PRESS_SHORT_THRESHOLD_MS    500
#define SHORT_BLINKS                5
#define LONG_BLINKS                 10
#define BLINK_HALF_CYCLE_TICKS      (100 / portTICK_PERIOD_MS)

/**
 * @brief Classifies the press and updates shared statistics.
 * @param duration Press duration in ms.
 * @return Number of blinks to perform (SHORT_BLINKS or LONG_BLINKS).
 */
static int classifyAndUpdateStats(int duration)
{
    setTotalPresses(getTotalPresses() + 1);

    if (duration < PRESS_SHORT_THRESHOLD_MS) {
        setShortPresses(getShortPresses() + 1);
        setSumShortDurations(getSumShortDurations() + duration);
        return SHORT_BLINKS;
    } else {
        setLongPresses(getLongPresses() + 1);
        setSumLongDurations(getSumLongDurations() + duration);
        return LONG_BLINKS;
    }
}

/**
 * @brief Blinks the yellow LED a given number of times (100 ms ON / 100 ms OFF).
 * @param blinkCount Number of ON/OFF cycles.
 */
static void blinkYellowLed(int blinkCount)
{
    for (int i = 0; i < blinkCount; i++) {
        digitalWrite(PIN_LED_YELLOW, HIGH);
        vTaskDelay(BLINK_HALF_CYCLE_TICKS);
        digitalWrite(PIN_LED_YELLOW, LOW);
        vTaskDelay(BLINK_HALF_CYCLE_TICKS);
    }
}

void vTaskPressHandler(void *pvParameters)
{
    (void)pvParameters;

    printf("[T2] PressHandler started - waiting for semaphore\n");

    for (;;) {
        if (xSemaphoreTake(xPressSemaphore, portMAX_DELAY) == pdTRUE) {

            int duration = getLastPressDuration();
            int blinkCount = classifyAndUpdateStats(duration);

            printf("[T2] %s press %d ms - total=%d, blinking %d times\n",
                   (duration < PRESS_SHORT_THRESHOLD_MS) ? "SHORT" : "LONG",
                   duration, getTotalPresses(), blinkCount);

            blinkYellowLed(blinkCount);

            printf("[T2] Yellow blink sequence done\n");
        }
    }
}
