/**
 * @file srvSysTimer.cpp
 * @brief System Timer Service - Implementation.
 *
 * Timer2 CTC mode: prescaler = 64, OCR2A = 249 → 1 ms tick.
 * The ISR only sets ready-flags; dispatch happens in the main loop.
 */

#include "srvSysTimer.h"
#include <avr/interrupt.h>

static SrvTask tasks[SRV_MAX_TASKS];
static int     taskCount = 0;

/* See srvSysTimer.h for documentation */
void srvSchedulerAddTask(SrvTaskFunc func, int periodMs, int offsetMs)
{
    if (taskCount >= SRV_MAX_TASKS) return;

    int periodTicks = periodMs / SRV_SYS_TICK_MS;
    int offsetTicks = offsetMs / SRV_SYS_TICK_MS;

    tasks[taskCount].func        = func;
    tasks[taskCount].periodTicks = periodTicks;
    tasks[taskCount].counter     = periodTicks - offsetTicks;
    tasks[taskCount].ready       = false;
    taskCount++;
}

/* See srvSysTimer.h for documentation */
void srvSysTimerInit()
{
    cli();

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;

    // OCR2A = (F_CPU / prescaler / freq) - 1 = (16 000 000 / 64 / 1000) - 1 = 249
    OCR2A = 249;

    TCCR2A |= (1 << WGM21);  // CTC mode
    TCCR2B |= (1 << CS22);   // 64 prescaler → 1 ms tick

    TIMSK2 |= (1 << OCIE2A); // enable compare-match A interrupt

    sei();
}

/* See srvSysTimer.h for documentation */
void srvSchedulerDispatch()
{
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].ready) {
            tasks[i].ready = false;
            tasks[i].func();
        }
    }
}

ISR(TIMER2_COMPA_vect)
{
    for (int i = 0; i < taskCount; i++) {
        if (--tasks[i].counter <= 0) {
            tasks[i].counter = tasks[i].periodTicks;
            tasks[i].ready   = true;
        }
    }
}
