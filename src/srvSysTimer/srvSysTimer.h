/**
 * @file srvSysTimer.h
 * @brief System Timer Service - Interface.
 *
 * Generic Timer2-based 1 ms tick scheduler for ATmega328P (Arduino Uno).
 * Timer2 CTC mode: prescaler = 64, OCR2A = 249 → 1 ms tick.
 *
 * The ISR only sets ready-flags.  Actual task functions are called from
 * the main loop by srvSchedulerDispatch(), keeping the system
 * non-preemptive.
 */

#ifndef SRV_SYS_TIMER_H
#define SRV_SYS_TIMER_H

#include <Arduino.h>

/** @brief Maximum number of tasks the scheduler can hold. */
#define SRV_MAX_TASKS 4

/** @brief System tick resolution in ms. */
#define SRV_SYS_TICK_MS 1

/** @brief Function pointer type for a scheduled task. */
typedef void (*SrvTaskFunc)(void);

/** @brief Descriptor for one periodic task. */
typedef struct {
    SrvTaskFunc func;           /**< Function to call when ready. */
    int         periodTicks;    /**< Period in ticks. */
    int         counter;        /**< Down-counter (decremented by ISR). */
    volatile bool ready;        /**< Set by ISR, cleared by dispatcher. */
} SrvTask;

/**
 * @brief Configures Timer2 for a 1 ms CTC interrupt and enables it.
 *        Must be called after srvSchedulerAddTask().
 */
void srvSysTimerInit();

/**
 * @brief Registers a task with the scheduler.
 * @param func       Task function pointer.
 * @param periodMs   Recurrence period in milliseconds.
 * @param offsetMs   Initial offset in milliseconds (staggers first fire).
 */
void srvSchedulerAddTask(SrvTaskFunc func, int periodMs, int offsetMs);

/**
 * @brief Dispatches all tasks whose ready flag has been set by the ISR.
 *        Call this from the main loop().
 */
void srvSchedulerDispatch();

#endif // SRV_SYS_TIMER_H
