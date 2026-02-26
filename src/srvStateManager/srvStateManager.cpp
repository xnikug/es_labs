/**
 * @file srvStateManager.cpp
 * @brief Signals Service - Implementation.
 */

#include "srvStateManager.h"

// Task 1 → Task 2 signals
volatile bool         sig_pressEvent    = false;
volatile unsigned int sig_pressDuration = 0;
volatile bool         sig_pressIsShort  = false;

// Task 2 → Task 3 signals
volatile unsigned int  sig_statTotal   = 0;
volatile unsigned int  sig_statShort   = 0;
volatile unsigned int  sig_statLong    = 0;
volatile unsigned long sig_statTotalMs = 0;

/* See srvStateManager.h for documentation */
void srvStateManagerInit()
{
    sig_pressEvent    = false;
    sig_pressDuration = 0;
    sig_pressIsShort  = false;

    sig_statTotal   = 0;
    sig_statShort   = 0;
    sig_statLong    = 0;
    sig_statTotalMs = 0;
}
