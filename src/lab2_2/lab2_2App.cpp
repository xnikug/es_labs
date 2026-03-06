/**
 * @file lab2_2App.cpp
 * @brief Lab 2.2 - FreeRTOS Button Statistics (Arduino Mega 2560).
 *
 * Three concurrent FreeRTOS tasks:
 *
 *  T1 - vTaskButtonLed   (1 tick poll, xTaskDelayUntil)
 *       Polls button; on release computes press duration, lights the green
 *       LED (~1 s) for short presses (<500 ms) or the red LED for long
 *       presses (>=500 ms) using a tick countdown, then gives
 *       xSemaphoreButtonPress to T2.
 *
 *  T2 - vTaskStatsYellow (semaphore-driven)
 *       Takes xSemaphoreButtonPress, updates press-duration statistics,
 *       then blinks the yellow LED 5x (short) or 10x (long) at
 *       100 ms ON / 100 ms OFF.
 *
 *  T3 - vTaskReport      (10 s recurrence, xTaskDelayUntil)
 *       Prints total/short/long press counts and average duration to STDIO,
 *       then resets all counters.
 *
 * Hardware (Arduino Mega 2560):
 *   PIN  3 - pushbutton (INPUT_PULLUP, active LOW -> GND)
 *   PIN 12 - green  LED (T1: short press indicator, ~1 s)
 *   PIN 10 - red    LED (T1: long  press indicator, ~1 s)
 *   PIN 11 - yellow LED (T2: blink sequence)
 */

#include "lab2_2App.h"
#include "srvSerialStdio/srvSerialStdio.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>

// ---------------------------------------------------------------------------
// Pin definitions
// ---------------------------------------------------------------------------

#define PIN_BUTTON      3   /**< Pushbutton: INPUT_PULLUP, active LOW.              */
#define PIN_LED_GREEN   12  /**< T1 indicator - on ~1 s for short press (<500 ms).  */
#define PIN_LED_RED     10  /**< T1 indicator - on ~1 s for long  press (>=500 ms). */
#define PIN_LED_YELLOW  11  /**< T2 indicator - blinks 5x short / 10x long press.  */

// ---------------------------------------------------------------------------
// FreeRTOS timing constants
// ---------------------------------------------------------------------------

/** Press duration threshold separating short from long presses (ms). */
#define PRESS_SHORT_THRESHOLD   500

// Polling period: 1 tick = 16 ms on Arduino Mega FreeRTOS
#define TASK_1_POLL_TICKS       1

// LED on-time duration (in ticks): 62 ticks * 16 ms = ~992 ms
#define LED_ON_TIME_TICKS       62

/** Number of yellow blinks for a short press. */
#define SHORT_BLINKS            5
/** Number of yellow blinks for a long press. */
#define LONG_BLINKS             10

// Blink half-cycle: 100 ms ON, 100 ms OFF
#define BLINK_HALF_CYCLE_TICKS  (100 / portTICK_PERIOD_MS)

// Report period: 10 seconds
#define REPORT_PERIOD_TICKS     (10000 / portTICK_PERIOD_MS)

// ---------------------------------------------------------------------------
// FreeRTOS synchronisation objects (module-scope)
// ---------------------------------------------------------------------------

/** Binary semaphore: T1 gives after LED expires, T2 takes. */
static SemaphoreHandle_t xSemaphoreButtonPress = NULL;
/** Mutex protecting last_press_duration (T1 writes, T2 reads). */
static SemaphoreHandle_t xMutexDuration        = NULL;
/** Mutex protecting statistics counters (T2 writes, T3 reads/resets). */
static SemaphoreHandle_t xMutexStats           = NULL;

// ---------------------------------------------------------------------------
// Shared state
// ---------------------------------------------------------------------------

static int last_press_duration  = 0;
static int total_presses        = 0;
static int short_presses        = 0;
static int long_presses         = 0;
static int sum_short_durations  = 0;
static int sum_long_durations   = 0;

// ---------------------------------------------------------------------------
// Task implementations
// ---------------------------------------------------------------------------

/**
 * @brief T1 - Button LED task (1 tick poll, xTaskDelayUntil).
 *
 * Monitors button state using a 2-state machine, measures press duration,
 * lights green (short) or red (long) LED using a tick countdown (LED_ON_TIME_TICKS),
 * then gives xSemaphoreButtonPress to T2 after the LED turns off.
 */
static void vTaskButtonLed(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    int button_state = 0;            // 0 = not pressed, 1 = pressed
    unsigned long press_start_time = 0;
    int press_duration = 0;

    // LED on-time counter: -1 = inactive, >=0 = counting down
    int led_on_counter = -1;
    int led_type = 0;                // 0 = none, 1 = green (short), 2 = red (long)

    printf("[T1] ButtonLed started\n");

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
                xSemaphoreTake(xMutexDuration, portMAX_DELAY);
                last_press_duration = press_duration;
                xSemaphoreGive(xMutexDuration);

                // Determine LED and start counter
                if (press_duration < PRESS_SHORT_THRESHOLD) {
                    // Short press: green LED
                    digitalWrite(PIN_LED_RED,   LOW);
                    digitalWrite(PIN_LED_GREEN, HIGH);
                    led_type = 1;
                    led_on_counter = LED_ON_TIME_TICKS;
                    printf("[T1] SHORT press %d ms - green LED on\n", press_duration);
                } else {
                    // Long press: red LED
                    digitalWrite(PIN_LED_GREEN, LOW);
                    digitalWrite(PIN_LED_RED,   HIGH);
                    led_type = 2;
                    led_on_counter = LED_ON_TIME_TICKS;
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
            xSemaphoreGive(xSemaphoreButtonPress);

            led_on_counter--; // Decrement to -1 to avoid repeated give
            led_type = 0;
        }

        vTaskDelayUntil(&xLastWakeTime, TASK_1_POLL_TICKS);
    }
}

/**
 * @brief T2 - Statistics & yellow blink task (semaphore-driven).
 *
 * Blocks on xSemaphoreButtonPress; on wake reads the press duration, updates
 * all statistics counters (mutex-protected), then blinks the yellow LED
 * SHORT_BLINKS or LONG_BLINKS times.
 */
static void vTaskStatsYellow(void *pvParameters)
{
    (void)pvParameters;

    printf("[T2] StatsYellow started - waiting for semaphore\n");

    for (;;) {
        // Block until T1 signals a completed button press
        if (xSemaphoreTake(xSemaphoreButtonPress, portMAX_DELAY) == pdTRUE) {

            // Read press duration (mutex-protected)
            xSemaphoreTake(xMutexDuration, portMAX_DELAY);
            int duration = last_press_duration;
            xSemaphoreGive(xMutexDuration);

            int blinks;

            // Update statistics based on press duration (mutex-protected)
            xSemaphoreTake(xMutexStats, portMAX_DELAY);
            total_presses++;
            if (duration < PRESS_SHORT_THRESHOLD) {
                // Short press
                short_presses++;
                sum_short_durations += duration;
                blinks = SHORT_BLINKS;
            } else {
                // Long press
                long_presses++;
                sum_long_durations += duration;
                blinks = LONG_BLINKS;
            }
            xSemaphoreGive(xMutexStats);

            printf("[T2] %s press %d ms - total=%d, blinking %d times\n",
                   (duration < PRESS_SHORT_THRESHOLD) ? "SHORT" : "LONG",
                   duration, total_presses, blinks);

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

/**
 * @brief T3 - Periodic statistics report task (10 s recurrence, xTaskDelayUntil).
 *
 * Every REPORT_PERIOD_TICKS prints total/short/long press counts and average
 * press duration to STDIO, then resets all counters.
 */
static void vTaskReport(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    printf("[T3] Report started - period=10000 ms\n");

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, REPORT_PERIOD_TICKS);

        // Read statistics (mutex-protected)
        xSemaphoreTake(xMutexStats, portMAX_DELAY);
        int tp  = total_presses;
        int sp  = short_presses;
        int lp  = long_presses;
        int ssd = sum_short_durations;
        int sld = sum_long_durations;
        xSemaphoreGive(xMutexStats);

        // Calculate average duration
        int avg_duration = 0;
        if (tp > 0) {
            avg_duration = (ssd + sld) / tp;
        }

        printf("\n========== TASK 3: PERIODIC REPORT (10s) ==========\n");
        printf("Total presses:      %d\n", tp);
        printf("Short presses:      %d (total duration: %d ms)\n", sp, ssd);
        printf("Long presses:       %d (total duration: %d ms)\n", lp, sld);
        printf("Average duration:   %d ms\n", avg_duration);
        printf("====================================================\n\n");

        // Reset statistics (mutex-protected)
        xSemaphoreTake(xMutexStats, portMAX_DELAY);
        total_presses       = 0;
        short_presses       = 0;
        long_presses        = 0;
        sum_short_durations = 0;
        sum_long_durations  = 0;
        xSemaphoreGive(xMutexStats);
    }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

/* See lab2_2App.h for documentation */
void lab2_2AppSetup()
{
    /* --- Serial STDIO ---------------------------------------------------- */
    srvSerialSetup(9600);

    printf("============================================\n");
    printf(" Lab 2.2 - FreeRTOS Button Statistics\n");
    printf("============================================\n");
    printf("HW: Arduino Mega 2560\n");
    printf("PIN_BUTTON=%d  LED_G=%d  LED_R=%d  LED_Y=%d\n",
           PIN_BUTTON, PIN_LED_GREEN, PIN_LED_RED, PIN_LED_YELLOW);
    printf("Tick period: %u ms\n", portTICK_PERIOD_MS);
    printf("Tasks:\n");
    printf("  T1 ButtonLed   1 tick poll     (xTaskDelayUntil, prio 3)\n");
    printf("  T2 StatsYellow event-driven    (xSemaphoreTake,  prio 2)\n");
    printf("  T3 Report      10 s period     (xTaskDelayUntil, prio 1)\n");
    printf("Short press threshold: %d ms\n", PRESS_SHORT_THRESHOLD);
    printf("Press the button to start!\n\n");

    /* --- GPIO ------------------------------------------------------------ */
    pinMode(PIN_BUTTON,     INPUT_PULLUP);
    pinMode(PIN_LED_GREEN,  OUTPUT);  digitalWrite(PIN_LED_GREEN,  LOW);
    pinMode(PIN_LED_RED,    OUTPUT);  digitalWrite(PIN_LED_RED,    LOW);
    pinMode(PIN_LED_YELLOW, OUTPUT);  digitalWrite(PIN_LED_YELLOW, LOW);

    /* --- FreeRTOS objects ------------------------------------------------ */
    xSemaphoreButtonPress = xSemaphoreCreateBinary();
    xMutexDuration        = xSemaphoreCreateMutex();
    xMutexStats           = xSemaphoreCreateMutex();

    if (xSemaphoreButtonPress == NULL || xMutexDuration == NULL || xMutexStats == NULL) {
        printf("FATAL: Failed to create FreeRTOS objects!\n");
        for (;;); /* halt */
    }

    /* --- Create tasks ----------------------------------------------------- */
    /*
     * Priority order: T1 highest (hard real-time button sampling),
     *                 T2 medium  (event processing & LED blink),
     *                 T3 lowest  (periodic reporting / logging).
     */
    xTaskCreate(vTaskButtonLed,   "T1_BtnLed",  500, NULL, 3, NULL);
    xTaskCreate(vTaskStatsYellow, "T2_Stats",   500, NULL, 2, NULL);
    xTaskCreate(vTaskReport,      "T3_Report",  500, NULL, 1, NULL);

    /* The FreeRTOS scheduler starts automatically after setup() returns. */
}

/* See lab2_2App.h for documentation */
void lab2_2AppLoop()
{
    /* Intentionally empty - the FreeRTOS scheduler owns the CPU. */
}
