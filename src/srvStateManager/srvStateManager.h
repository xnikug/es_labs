/**
 * @file srvStateManager.h
 * @brief Signals Service - Interface (provider/consumer shared variables).
 *
 * All signals are declared volatile so ISR-context writes are visible
 * to the main-loop consumer (Task 3 / idle).
 *
 * Data flow:
 *   Task 1 (provider) → sig_press*       → Task 2 (consumer)
 *   Task 2 (provider) → sig_stat*        → Task 3 (consumer)
 */

#ifndef SRV_STATE_MANAGER_H
#define SRV_STATE_MANAGER_H

#include <Arduino.h>

// ---------------------------------------------------------------------------
// Task 1 → Task 2 signals (button press event)
// ---------------------------------------------------------------------------

/** @brief True for exactly one Task 2 cycle after a press is released. */
extern volatile bool sig_pressEvent;

/** @brief Duration of the last completed press in milliseconds. */
extern volatile unsigned int sig_pressDuration;

/** @brief True if last press was short (< 500 ms). */
extern volatile bool sig_pressIsShort;

// ---------------------------------------------------------------------------
// Task 2 → Task 3 signals (cumulative statistics)
// ---------------------------------------------------------------------------

/** @brief Total number of completed presses. */
extern volatile unsigned int sig_statTotal;

/** @brief Number of short presses. */
extern volatile unsigned int sig_statShort;

/** @brief Number of long presses. */
extern volatile unsigned int sig_statLong;

/** @brief Sum of all press durations (for average). */
extern volatile unsigned long sig_statTotalMs;

// ---------------------------------------------------------------------------
// Init
// ---------------------------------------------------------------------------

/**
 * @brief Resets all signals to zero / false.
 */
void srvStateManagerInit();

#endif // SRV_STATE_MANAGER_H
