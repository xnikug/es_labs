/**
 * @file lab2_2ButtonMonitor.cpp
 * @brief Lab 2.2 - T1 Button monitor task (implementation).
 *
 * Behaviour:
 *  - Polls the button every POLL_TICKS (1 tick = ~16 ms) via xTaskDelayUntil.
 *  - Uses a 2-state machine (idle / pressed) to detect press and release edges.
 *  - On release: stores duration, lights green LED (short) or red LED (long)
 *    for LED_ON_TICKS (~992 ms) using a countdown counter.
 *  - When the counter expires: turns off the LED and gives xPressSemaphore
 *    to unblock T2.
 */

#include "lab2_2ButtonMonitor.h"
#include "lab2_2App.h"
#include <Arduino.h>
#include <stdio.h>

#define PRESS_SHORT_THRESHOLD_MS    500

// Polling period: 1 tick = 16 ms on Arduino Mega FreeRTOS
#define POLL_TICKS                  1

// LED on-time duration: 62 ticks * 16 ms = ~992 ms
#define LED_ON_TICKS                62

/* See lab2_2ButtonMonitor.h for documentation */
void vTaskButtonMonitor(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    int button_state = 0;            // 0 = idle, 1 = pressed
    unsigned long press_start_time = 0;
    int press_duration = 0;

    // LED on-time counter: -1 = inactive, >=0 = counting down
    int led_on_counter = -1;
    int led_type = 0;                // 0 = none, 1 = green (short), 2 = red (long)

    printf("[T1] ButtonMonitor started\n");

    for (;;) {
        int button_is_pressed_now = (digitalRead(PIN_BUTTON) == LOW);

        // State: Idle (button not pressed)
        if (button_state == 0) {
            // Transition: button pressed
            if (button_is_pressed_now) {
                button_state = 1;
                press_start_time = millis();
            }
        }
        // State: Button pressed
        else if (button_state == 1) {
            // Transition: button released
            if (!button_is_pressed_now) {
                button_state = 0;
                press_duration = (int)(millis() - press_start_time);

                // Store duration in shared variable (mutex-protected)
                setLastPressDuration(press_duration);

                // Determine LED and start counter
                if (press_duration < PRESS_SHORT_THRESHOLD_MS) {
                    // Short press: green LED
                    digitalWrite(PIN_LED_RED,   LOW);
                    digitalWrite(PIN_LED_GREEN, HIGH);
                    led_type = 1;
                    led_on_counter = LED_ON_TICKS;
                    printf("[T1] SHORT press %d ms - green LED on\n", press_duration);
                } else {
                    // Long press: red LED
                    digitalWrite(PIN_LED_GREEN, LOW);
                    digitalWrite(PIN_LED_RED,   HIGH);
                    led_type = 2;
                    led_on_counter = LED_ON_TICKS;
                    printf("[T1] LONG press %d ms - red LED on\n", press_duration);
                }
            }
        }

        // LED on-time counter logic
        if (led_on_counter > 0) {
            led_on_counter--;
            // Keep LED on (already turned on above)
        } else if (led_on_counter == 0) {
            // Counter expired: turn off LED and signal T2
            if (led_type == 1) {
                digitalWrite(PIN_LED_GREEN, LOW);
            } else if (led_type == 2) {
                digitalWrite(PIN_LED_RED, LOW);
            }

            // Give semaphore to T2 only AFTER LED is off
            printf("[T1] LED off - giving semaphore to T2\n");
            xSemaphoreGive(xPressSemaphore);

            led_on_counter--; // Decrement to -1 to avoid repeated give
            led_type = 0;
        }

        vTaskDelayUntil(&xLastWakeTime, POLL_TICKS);
    }
}
