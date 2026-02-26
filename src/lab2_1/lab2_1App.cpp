#include "lab2_1App.h"
#include "srvStateManager/srvStateManager.h"
#include "srvSysTimer/srvSysTimer.h"
#include "ddButton/ddButton.h"
#include "ddLed/ddLed.h"
#include "srvSerialStdio/srvSerialStdio.h"
#include <stdio.h>


#define PIN_LED_GREEN   4   /**< Short-press feedback. */
#define PIN_LED_RED     5   /**< Long-press feedback. */
#define PIN_LED_YELLOW  6   /**< Blink sequence (5× short, 10× long). */
#define PIN_BUTTON      7   /**< Push button (INPUT_PULLUP, active LOW). */

// --- Timing constants -------------------------------------------------------

#define SHORT_PRESS_THRESHOLD_MS  500   /**< < 500 ms = short press. */
#define FEEDBACK_LED_MS           500   /**< Green / Red ON time after press. */

/** Task 2 runs every 50 ms; blink timing expressed in task-ticks. */
#define TASK2_PERIOD_MS           50
#define BLINK_HALF_TICKS          2     /**< 2 × 50 ms = 100 ms per half-cycle. */
#define SHORT_PRESS_BLINKS        5
#define LONG_PRESS_BLINKS         10

// --- Task schedule ----------------------------------------------------------

#define T1_PERIOD_MS   50
#define T1_OFFSET_MS   0
#define T2_PERIOD_MS   50
#define T2_OFFSET_MS   5
#define T3_PERIOD_MS   10000
#define T3_OFFSET_MS   10

// ============================================================================
// Task 1 – Button Monitor (PROVIDER)
// ============================================================================

static bool     t1_lastPressed   = false;
static unsigned long t1_pressStart = 0;

static void task1ButtonMonitor()
{
    bool pressed = ddButtonIsPressed(PIN_BUTTON);

    if (pressed && !t1_lastPressed) {
        // Falling edge – button just pressed
        t1_pressStart = millis();
    }
    else if (!pressed && t1_lastPressed) {
        // Rising edge – button just released
        unsigned long duration = millis() - t1_pressStart;

        // Provider writes
        sig_pressDuration = (unsigned int)duration;
        sig_pressIsShort  = (duration < SHORT_PRESS_THRESHOLD_MS);
        sig_pressEvent    = true;   // must be set LAST (flag for consumer)
    }

    t1_lastPressed = pressed;
}

// ============================================================================
// Task 2 – Stats + Blink (CONSUMER of T1 / PROVIDER of stats)
// ============================================================================

/** Blink state-machine states. */
enum BlinkState { BLINK_IDLE, BLINK_FEEDBACK, BLINK_BLINKING };

static BlinkState t2_state          = BLINK_IDLE;
static int        t2_halfCycles     = 0;    /**< Remaining half-cycles. */
static int        t2_halfTicks      = 0;    /**< Tick counter within half-cycle. */
static int        t2_feedbackTicks  = 0;    /**< Feedback LED countdown. */

static void task2StatsAndBlink()
{
    // ---- Consume press event from Task 1 ----
    if (sig_pressEvent && t2_state == BLINK_IDLE) {
        sig_pressEvent = false;                     // acknowledge

        // Update statistics (provider writes)
        sig_statTotal++;
        sig_statTotalMs += sig_pressDuration;

        if (sig_pressIsShort) {
            sig_statShort++;
            ddLedOn(PIN_LED_GREEN);
            t2_halfCycles = SHORT_PRESS_BLINKS * 2;
        } else {
            sig_statLong++;
            ddLedOn(PIN_LED_RED);
            t2_halfCycles = LONG_PRESS_BLINKS * 2;
        }

        t2_feedbackTicks = FEEDBACK_LED_MS / TASK2_PERIOD_MS;   // 500/50 = 10
        t2_state = BLINK_FEEDBACK;
        return;
    }

    // ---- Feedback LED phase (green or red ON for 500 ms) ----
    if (t2_state == BLINK_FEEDBACK) {
        if (--t2_feedbackTicks <= 0) {
            ddLedOff(PIN_LED_GREEN);
            ddLedOff(PIN_LED_RED);
            // Start yellow blink sequence
            ddLedOn(PIN_LED_YELLOW);
            t2_halfTicks = BLINK_HALF_TICKS;
            t2_state = BLINK_BLINKING;
        }
        return;
    }

    // ---- Yellow blink phase ----
    if (t2_state == BLINK_BLINKING) {
        if (--t2_halfTicks <= 0) {
            ddLedToggle(PIN_LED_YELLOW);
            t2_halfCycles--;

            if (t2_halfCycles <= 0) {
                ddLedOff(PIN_LED_YELLOW);
                t2_state = BLINK_IDLE;
            } else {
                t2_halfTicks = BLINK_HALF_TICKS;
            }
        }
        return;
    }
}

// ============================================================================
// Task 3 – Report (CONSUMER – printf, runs every 10 s)
// ============================================================================

static void task3Report()
{
    unsigned int total = sig_statTotal;
    unsigned int shortP = sig_statShort;
    unsigned int longP  = sig_statLong;
    unsigned long totalMs = sig_statTotalMs;

    unsigned int avgMs = (total > 0) ? (unsigned int)(totalMs / total) : 0;

    printf("=== Report (10s) ===\n");
    printf("Total presses : %u\n", total);
    printf("Short presses : %u\n", shortP);
    printf("Long  presses : %u\n", longP);
    printf("Avg duration  : %u ms\n", avgMs);
    printf("====================\n\n");

    // Reset statistics for the next 10 s window
    sig_statTotal   = 0;
    sig_statShort   = 0;
    sig_statLong    = 0;
    sig_statTotalMs = 0;
}

// ============================================================================
// Setup / Loop
// ============================================================================

/* See lab2_1App.h for documentation */
void lab2_1AppSetup()
{
    srvSerialSetup(9600);

    ddButtonInit(PIN_BUTTON);
    ddLedInit(PIN_LED_GREEN);
    ddLedInit(PIN_LED_RED);
    ddLedInit(PIN_LED_YELLOW);

    srvStateManagerInit();

    // Register tasks with the scheduler
    srvSchedulerAddTask(task1ButtonMonitor, T1_PERIOD_MS, T1_OFFSET_MS);
    srvSchedulerAddTask(task2StatsAndBlink, T2_PERIOD_MS, T2_OFFSET_MS);
    srvSchedulerAddTask(task3Report,        T3_PERIOD_MS, T3_OFFSET_MS);

    srvSysTimerInit();  // start Timer2 ISR last

    printf("Lab 2.1 ready. Press the button!\n");
    printf("T1: ButtonMonitor rec=%dms off=%dms\n", T1_PERIOD_MS, T1_OFFSET_MS);
    printf("T2: StatsAndBlink rec=%dms off=%dms\n", T2_PERIOD_MS, T2_OFFSET_MS);
    printf("T3: Report        rec=%dms off=%dms\n\n", T3_PERIOD_MS, T3_OFFSET_MS);
}

/* See lab2_1App.h for documentation */
void lab2_1AppLoop()
{
    srvSchedulerDispatch();
}
