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
#define POLL_TICKS                  1
#define LED_ON_TICKS                62

/** @brief Returns true when the button is currently pressed (active LOW). */
static int isButtonPressed()
{
    return (digitalRead(PIN_BUTTON) == LOW);
}

/**
 * @brief Activates the indicator LED for the given press duration.
 * @param pressDuration Duration of the button press in ms.
 * @return LED type: 1 = green (short), 2 = red (long).
 */
static int activateIndicatorLed(int pressDuration)
{
    if (pressDuration < PRESS_SHORT_THRESHOLD_MS) {
        digitalWrite(PIN_LED_RED,   LOW);
        digitalWrite(PIN_LED_GREEN, HIGH);
        printf("[T1] SHORT press %d ms - green LED on\n", pressDuration);
        return 1;
    } else {
        digitalWrite(PIN_LED_GREEN, LOW);
        digitalWrite(PIN_LED_RED,   HIGH);
        printf("[T1] LONG press %d ms - red LED on\n", pressDuration);
        return 2;
    }
}

/**
 * @brief Turns off the indicator LED corresponding to the given type.
 * @param ledType 1 = green, 2 = red.
 */
static void deactivateIndicatorLed(int ledType)
{
    if (ledType == 1) {
        digitalWrite(PIN_LED_GREEN, LOW);
    } else if (ledType == 2) {
        digitalWrite(PIN_LED_RED, LOW);
    }
}

void vTaskButtonMonitor(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    int buttonState          = 0;     // 0 = idle, 1 = pressed
    unsigned long pressStartTime = 0;
    int pressDuration        = 0;
    int ledOnCounter         = -1;    // -1 = inactive, >=0 = counting down
    int ledType              = 0;     // 0 = none, 1 = green, 2 = red

    printf("[T1] ButtonMonitor started\n");

    for (;;) {
        int pressed = isButtonPressed();

        // State: Idle
        if (buttonState == 0) {
            if (pressed) {
                buttonState = 1;
                pressStartTime = millis();
            }
        }
        // State: Pressed
        else if (buttonState == 1) {
            if (!pressed) {
                buttonState = 0;
                pressDuration = (int)(millis() - pressStartTime);

                setLastPressDuration(pressDuration);

                ledType = activateIndicatorLed(pressDuration);
                ledOnCounter = LED_ON_TICKS;
            }
        }

        // LED countdown logic
        if (ledOnCounter > 0) {
            ledOnCounter--;
        } else if (ledOnCounter == 0) {
            deactivateIndicatorLed(ledType);

            printf("[T1] LED off - giving semaphore to T2\n");
            xSemaphoreGive(xPressSemaphore);

            ledOnCounter--;
            ledType = 0;
        }

        vTaskDelayUntil(&xLastWakeTime, POLL_TICKS);
    }
}
