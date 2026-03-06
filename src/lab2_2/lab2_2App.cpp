/**
 * @file lab2_2App.cpp
 * @brief Lab 2.2 – FreeRTOS Preemptive OS (Arduino Mega 2560).
 *
 * Three concurrent FreeRTOS tasks:
 *
 *  T1 – vTaskButtonLed  (10 ms recurrence, xTaskDelayUntil)
 *       Polls button; on press lights green LED for 1 s and gives the
 *       binary semaphore xButtonSemaphore.
 *
 *  T2 – vTaskSync       (semaphore-driven, 50 ms internal delay)
 *       Waits for xButtonSemaphore, increments global N, enqueues N bytes
 *       (1 … N) followed by a 0-terminator into xDataQueue using
 *       xQueueSendToFront(), then blinks the red LED N times
 *       (300 ms ON / 500 ms OFF).
 *
 *  T3 – vTaskAsync      (200 ms recurrence)
 *       Drains xDataQueue with xQueueReceive(); prints each byte and starts
 *       a new line on every 0-terminator.  Also blinks the yellow LED once
 *       per read cycle that contains data.
 *
 * Hardware (Arduino Mega 2560):
 *   PIN  3 – pushbutton (INPUT_PULLUP, active LOW → GND)
 *   PIN 12 – green  LED  (T1 indicator)
 *   PIN 11 – red    LED  (T2 blink sequence)
 *   PIN 10 – yellow LED  (T3 RX activity)
 */

#include "lab2_2App.h"
#include "srvSerialStdio/srvSerialStdio.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
#include <stdio.h>

// ---------------------------------------------------------------------------
// Pin definitions
// ---------------------------------------------------------------------------

#define PIN_BUTTON      3   /**< Pushbutton: INPUT_PULLUP, active LOW.          */
#define PIN_LED_GREEN   12  /**< T1 indicator – on 1 s after button press.      */
#define PIN_LED_RED     11  /**< T2 indicator – blinks N times after semaphore. */
#define PIN_LED_YELLOW  10  /**< T3 indicator – blinks on each non-empty drain. */

// ---------------------------------------------------------------------------
// FreeRTOS timing constants
// ---------------------------------------------------------------------------

/** T1 recurrence: 10 ms */
#define T1_PERIOD_MS        10
/** T2 startup delay before queue-fill + blink loop (ms). */
#define T2_PRE_DELAY_MS     50
/** T2 red LED ON time per blink (ms). */
#define T2_BLINK_ON_MS      300
/** T2 red LED OFF time per blink (ms). */
#define T2_BLINK_OFF_MS     500
/** T3 poll period (ms). */
#define T3_PERIOD_MS        200
/** Green LED on-time after button press (ms). */
#define BTN_LED_ON_MS       1000
/** Maximum bytes storable in xDataQueue. */
#define QUEUE_SIZE          128

// ---------------------------------------------------------------------------
// FreeRTOS synchronisation objects (module-scope)
// ---------------------------------------------------------------------------

static SemaphoreHandle_t xButtonSemaphore = NULL;
static QueueHandle_t     xDataQueue       = NULL;

// ---------------------------------------------------------------------------
// Shared state
// ---------------------------------------------------------------------------

/** Monotonically increasing press counter; used as byte-sequence length. */
static volatile int N = 0;

// ---------------------------------------------------------------------------
// Task implementations
// ---------------------------------------------------------------------------

/**
 * @brief T1 – Button LED task (recurrence 10 ms, xTaskDelayUntil).
 *
 * On every rising edge (press detected) the green LED turns on for 1 s and
 * xButtonSemaphore is given to T2.  xTaskDelayUntil ensures a precise 10 ms
 * period regardless of execution time.
 */
static void vTaskButtonLed(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod  = pdMS_TO_TICKS(T1_PERIOD_MS);

    bool lastState = false; /* previous debounced state */

    printf("[T1] ButtonLed started – period=%d ms\n", T1_PERIOD_MS);

    for (;;) {
        bool pressed = (digitalRead(PIN_BUTTON) == LOW);

        /* Rising-edge detection (LOW → first LOW after HIGH) */
        if (pressed && !lastState) {
            printf("[T1] Button pressed – lighting green LED, giving semaphore\n");

            /* Light green LED */
            digitalWrite(PIN_LED_GREEN, HIGH);

            /* Signal T2 */
            xSemaphoreGive(xButtonSemaphore);

            /* Keep LED on for 1 s within this task */
            vTaskDelay(pdMS_TO_TICKS(BTN_LED_ON_MS));

            digitalWrite(PIN_LED_GREEN, LOW);
        }

        lastState = pressed;

        /* Precise 10 ms tick – accounts for execution time above */
        xTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * @brief T2 – Synchronous task (event-driven via semaphore).
 *
 * Waits indefinitely for xButtonSemaphore:
 *  1. Increments N.
 *  2. Enqueues bytes N, N-1 … 1 (all via xQueueSendToFront), then enqueues
 *     a 0-terminator also via xQueueSendToFront.  Reading T3 will therefore
 *     dequeue in the order: 0, 1, 2 … N (ascending, newline-first).
 *  3. Delays T2_PRE_DELAY_MS (50 ms) to let T3 start reading.
 *  4. Blinks red LED N times (300 ms ON / 500 ms OFF).
 */
static void vTaskSync(void *pvParameters)
{
    (void)pvParameters;

    printf("[T2] Sync task started – waiting for semaphore\n");

    for (;;) {
        /* Block until T1 signals a button press */
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE) {

            N++; /* increment shared counter */

            printf("[T2] Semaphore received – N=%d, queuing %d bytes\n", N, N);

            /*
             * Enqueue bytes using xQueueSendToFront in reverse order
             * (N, N-1 … 1) so they sit in the queue as [1, 2 … N].
             * Finally push the 0-terminator to the very front so the
             * full queue reads [0, 1, 2 … N].
             */
            for (int i = N; i >= 1; i--) {
                uint8_t byte = (uint8_t)i;
                if (xQueueSendToFront(xDataQueue, &byte,
                                      pdMS_TO_TICKS(10)) != pdTRUE) {
                    printf("[T2] WARNING: queue full, dropped byte %d\n", i);
                }
            }
            /* 0-terminator goes to the very front */
            uint8_t terminator = 0;
            xQueueSendToFront(xDataQueue, &terminator, pdMS_TO_TICKS(10));

            /* Brief pause before blink sequence */
            vTaskDelay(pdMS_TO_TICKS(T2_PRE_DELAY_MS));

            /* Blink red LED N times */
            printf("[T2] Blinking red LED %d time(s)\n", N);
            for (int i = 0; i < N; i++) {
                digitalWrite(PIN_LED_RED, HIGH);
                vTaskDelay(pdMS_TO_TICKS(T2_BLINK_ON_MS));
                digitalWrite(PIN_LED_RED, LOW);
                vTaskDelay(pdMS_TO_TICKS(T2_BLINK_OFF_MS));
            }
            printf("[T2] Blink sequence done\n");
        }
    }
}

/**
 * @brief T3 – Asynchronous task (polls queue every 200 ms).
 *
 * Drains xDataQueue with xQueueReceive().  Each byte is printed as a
 * decimal value.  A 0-byte causes a line break.  A yellow LED blinks once
 * per non-empty drain cycle to give visual feedback on the board.
 */
static void vTaskAsync(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod  = pdMS_TO_TICKS(T3_PERIOD_MS);

    printf("[T3] Async task started – polling queue every %d ms\n",
           T3_PERIOD_MS);

    for (;;) {
        uint8_t byte;
        bool    hadData = false;

        /* Drain all available bytes in the queue this cycle */
        while (xQueueReceive(xDataQueue, &byte, 0) == pdTRUE) {
            if (!hadData) {
                /* First byte of a new batch – print header */
                printf("[T3] Received: ");
                hadData = true;
            }

            if (byte == 0) {
                /* 0-terminator → new line */
                printf("\n[T3] ------- (N=%d) -------\n", N);
            } else {
                printf("%3d", (int)byte);
            }
        }

        if (hadData) {
            /* End of this batch */
            printf("\n");

            /* Blink yellow LED once as RX-activity indicator */
            digitalWrite(PIN_LED_YELLOW, HIGH);
            vTaskDelay(pdMS_TO_TICKS(100));
            digitalWrite(PIN_LED_YELLOW, LOW);
        }

        xTaskDelayUntil(&xLastWakeTime, xPeriod);
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
    printf(" Lab 2.2 – FreeRTOS Preemptive Scheduler\n");
    printf("============================================\n");
    printf("HW: Arduino Mega 2560\n");
    printf("PIN_BUTTON=%d  LED_G=%d  LED_R=%d  LED_Y=%d\n",
           PIN_BUTTON, PIN_LED_GREEN, PIN_LED_RED, PIN_LED_YELLOW);
    printf("Tasks:\n");
    printf("  T1 ButtonLed  period=%d ms  (xTaskDelayUntil)\n", T1_PERIOD_MS);
    printf("  T2 Sync       event-driven  (xSemaphoreTake)\n");
    printf("  T3 Async      period=%d ms  (xTaskDelayUntil)\n", T3_PERIOD_MS);
    printf("Press the button to start!\n\n");

    /* --- GPIO ------------------------------------------------------------ */
    pinMode(PIN_BUTTON,     INPUT_PULLUP);
    pinMode(PIN_LED_GREEN,  OUTPUT);  digitalWrite(PIN_LED_GREEN,  LOW);
    pinMode(PIN_LED_RED,    OUTPUT);  digitalWrite(PIN_LED_RED,    LOW);
    pinMode(PIN_LED_YELLOW, OUTPUT);  digitalWrite(PIN_LED_YELLOW, LOW);

    /* --- FreeRTOS objects ------------------------------------------------ */
    xButtonSemaphore = xSemaphoreCreateBinary();
    xDataQueue       = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));

    if (xButtonSemaphore == NULL || xDataQueue == NULL) {
        printf("FATAL: Failed to create FreeRTOS objects!\n");
        for (;;); /* halt */
    }

    /* --- Create tasks ----------------------------------------------------- */
    /*
     * Stack sizes are intentionally generous for AVR; reduce if RAM is tight.
     * Priority order: T1 highest (hard real-time button sampling),
     *                 T2 medium  (event processing),
     *                 T3 lowest  (display/logging).
     */
    xTaskCreate(vTaskButtonLed, "T1_BtnLed", 256, NULL, 3, NULL);
    xTaskCreate(vTaskSync,      "T2_Sync",   384, NULL, 2, NULL);
    xTaskCreate(vTaskAsync,     "T3_Async",  384, NULL, 1, NULL);

    /* The FreeRTOS scheduler starts automatically after setup() returns. */
}

/* See lab2_2App.h for documentation */
void lab2_2AppLoop()
{
    /* Intentionally empty – the FreeRTOS scheduler owns the CPU. */
}
