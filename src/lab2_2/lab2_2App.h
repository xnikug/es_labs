/**
 * @file lab2_2App.h
 * @brief Application interface for Lab 2.2 – FreeRTOS Preemptive OS.
 *
 * Runs 3 concurrent FreeRTOS tasks on an Arduino Mega 2560:
 *
 * | Task | Function         | Period   | Sync           | Role                             |
 * |------|------------------|----------|----------------|----------------------------------|
 * | T1   | vTaskButtonLed   | 10 ms    | –              | Detects button press, gives sem  |
 * | T2   | vTaskSync        | event    | xSemaphoreTake | Increments N, fills queue, blinks|
 * | T3   | vTaskAsync       | 200 ms   | –              | Drains queue, prints to STDIO    |
 *
 * Hardware (Arduino Mega):
 *   PIN 3  – pushbutton (INPUT_PULLUP, active LOW)
 *   PIN 12 – green  LED (Task 1 indicator, on for 1 s per press)
 *   PIN 11 – red    LED (Task 2 blink: N times, 300 ms ON / 500 ms OFF)
 *   PIN 10 – yellow LED (Task 3 RX activity blink)
 *
 * Synchronisation:
 *   xButtonSemaphore – binary sempahore, T1 gives, T2 takes
 *   xDataQueue       – byte queue (capacity QUEUE_SIZE), T2 fills, T3 drains
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
