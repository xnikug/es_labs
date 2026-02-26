/**
 * @file lab2_1App.h
 * @brief Application interface for Lab 2.1 - Sequential Non-Preemptive Scheduler.
 *
 * 1 button + 3 LEDs on Arduino Uno.  Measures button press duration,
 * classifies short (< 500 ms) vs long (≥ 500 ms), gives LED feedback,
 * and reports statistics every 10 s via printf().
 *
 * Scheduler: Timer2 CTC, prescaler=64, OCR2A=249 → 1 ms tick.
 * Tasks are dispatched from loop() via srvSchedulerDispatch().
 *
 * | Task | Function              | Period    | Offset | Role                       |
 * |------|-----------------------|-----------|--------|----------------------------|
 * | T1   | task1ButtonMonitor    | 50 ms     | 0 ms   | PROVIDER – samples button  |
 * | T2   | task2StatsAndBlink    | 50 ms     | 5 ms   | CONSUMER T1 / PROVIDER     |
 * | T3   | task3Report           | 10 000 ms | 10 ms  | CONSUMER – printf report   |
 *
 * stdout → UART via srvSerialStdio.
 */

#ifndef LAB2_1_APP_H
#define LAB2_1_APP_H

/**
 * @brief Initialises peripherals, serial STDIO, signals, and scheduler.
 */
void lab2_1AppSetup();

/**
 * @brief Main loop – calls srvSchedulerDispatch() (non-preemptive).
 */
void lab2_1AppLoop();

#endif // LAB2_1_APP_H
