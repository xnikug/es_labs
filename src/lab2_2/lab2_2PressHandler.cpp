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

// Blink half-cycle: 100 ms ON, 100 ms OFF
#define BLINK_HALF_CYCLE_TICKS      (100 / portTICK_PERIOD_MS)

/* See lab2_2PressHandler.h for documentation */
void vTaskPressHandler(void *pvParameters)
{
    (void)pvParameters;

    printf("[T2] PressHandler started - waiting for semaphore\n");

    for (;;) {
        // Block until T1 signals a completed button press
        if (xSemaphoreTake(xPressSemaphore, portMAX_DELAY) == pdTRUE) {

            // Read press duration (mutex-protected)
            int duration = getLastPressDuration();

            int blinks;

            // Update statistics based on press duration (mutex-protected)
            setTotalPresses(getTotalPresses() + 1);

            if (duration < PRESS_SHORT_THRESHOLD_MS) {
                // Short press
                setShortPresses(getShortPresses() + 1);
                setSumShortDurations(getSumShortDurations() + duration);
                blinks = SHORT_BLINKS;
            } else {
                // Long press
                setLongPresses(getLongPresses() + 1);
                setSumLongDurations(getSumLongDurations() + duration);
                blinks = LONG_BLINKS;
            }

            printf("[T2] %s press %d ms - total=%d, blinking %d times\n",
                   (duration < PRESS_SHORT_THRESHOLD_MS) ? "SHORT" : "LONG",
                   duration, getTotalPresses(), blinks);

            // Blink yellow LED
            for (int i = 0; i < blinks; i++) {
                digitalWrite(PIN_LED_YELLOW, HIGH);
                vTaskDelay(BLINK_HALF_CYCLE_TICKS);
                digitalWrite(PIN_LED_YELLOW, LOW);
                vTaskDelay(BLINK_HALF_CYCLE_TICKS);
            }

            printf("[T2] Yellow blink sequence done\n");
        }
    }
}
