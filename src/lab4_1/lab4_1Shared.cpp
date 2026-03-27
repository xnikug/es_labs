/**
 * @file lab4_1Shared.cpp
 * @brief Shared state for Lab 4.1 relay control app.
 */

#include "lab4_1Shared.h"
#include <string.h>

#define LAB4_1_INVALID_ALERT_CYCLES 4

Lab4_1State_t gLab4_1State = {false, false, false, false, 0, true, "none", LAB4_1_SRC_NONE, 0, 0, 0};
SemaphoreHandle_t xLab4_1StateMutex = NULL;

static void setLastCommand(const char *cmd)
{
    strncpy(gLab4_1State.lastCommand, cmd, sizeof(gLab4_1State.lastCommand) - 1);
    gLab4_1State.lastCommand[sizeof(gLab4_1State.lastCommand) - 1] = '\0';
}

void lab4_1SharedInit()
{
    xLab4_1StateMutex = xSemaphoreCreateMutex();
}

void lab4_1SharedSetRawCommand(bool relayOn, const char *cmd, Lab4_1CommandSource_t src)
{
    if (xSemaphoreTake(xLab4_1StateMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        return;
    }

    gLab4_1State.rawRequestedOn = relayOn;
    gLab4_1State.lastCommandValid = true;
    gLab4_1State.lastSource = src;
    gLab4_1State.validCommandCount++;
    setLastCommand(cmd);

    xSemaphoreGive(xLab4_1StateMutex);
}

void lab4_1SharedSetDebouncedCommand(bool relayOn, uint8_t debounceCounter)
{
    if (xSemaphoreTake(xLab4_1StateMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        return;
    }

    gLab4_1State.relayRequestedOn = relayOn;
    gLab4_1State.debounceCounter = debounceCounter;

    xSemaphoreGive(xLab4_1StateMutex);
}

void lab4_1SharedSetInvalidCommand(const char *cmd, Lab4_1CommandSource_t src)
{
    if (xSemaphoreTake(xLab4_1StateMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        return;
    }

    gLab4_1State.lastCommandValid = false;
    gLab4_1State.lastSource = src;
    gLab4_1State.invalidCommandCount++;
    gLab4_1State.invalidAlertCycles = LAB4_1_INVALID_ALERT_CYCLES;
    setLastCommand(cmd);

    xSemaphoreGive(xLab4_1StateMutex);
}

Lab4_1State_t lab4_1SharedGetSnapshot()
{
    Lab4_1State_t snapshot = gLab4_1State;

    if (xSemaphoreTake(xLab4_1StateMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        return snapshot;
    }

    snapshot = gLab4_1State;
    xSemaphoreGive(xLab4_1StateMutex);
    return snapshot;
}

void lab4_1SharedSetAppliedRelayState(bool relayOn)
{
    if (xSemaphoreTake(xLab4_1StateMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        return;
    }

    if (gLab4_1State.relayAppliedOn != relayOn) {
        gLab4_1State.relayChangePendingAck = true;
    }
    gLab4_1State.relayAppliedOn = relayOn;

    xSemaphoreGive(xLab4_1StateMutex);
}

void lab4_1SharedConsumeAlertCycle()
{
    if (xSemaphoreTake(xLab4_1StateMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        return;
    }

    if (gLab4_1State.invalidAlertCycles > 0) {
        gLab4_1State.invalidAlertCycles--;
    }
    gLab4_1State.relayChangePendingAck = false;

    xSemaphoreGive(xLab4_1StateMutex);
}
