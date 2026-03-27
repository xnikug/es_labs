/**
 * @file lab4_1Shared.h
 * @brief Shared state for Lab 4.1 relay control app.
 */

#ifndef LAB4_1_SHARED_H
#define LAB4_1_SHARED_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define LAB4_1_FAN_CMD_OFF 0
#define LAB4_1_FAN_CMD_ON 1
#define LAB4_1_DEBOUNCE_MAX_COUNTER 20

typedef enum {
    LAB4_1_SRC_NONE = 0,
    LAB4_1_SRC_SERIAL
} Lab4_1CommandSource_t;

typedef struct {
    bool rawRequestedOn;
    bool relayRequestedOn;
    bool relayAppliedOn;
    bool relayChangePendingAck;
    uint8_t debounceCounter;
    bool lastCommandValid;
    char lastCommand[17];
    Lab4_1CommandSource_t lastSource;
    uint32_t validCommandCount;
    uint32_t invalidCommandCount;
    uint8_t invalidAlertCycles;
} Lab4_1State_t;

extern Lab4_1State_t gLab4_1State;
extern SemaphoreHandle_t xLab4_1StateMutex;

void lab4_1SharedInit();
void lab4_1SharedSetRawCommand(bool relayOn, const char *cmd, Lab4_1CommandSource_t src);
void lab4_1SharedSetInvalidCommand(const char *cmd, Lab4_1CommandSource_t src);
void lab4_1SharedSetDebouncedCommand(bool relayOn, uint8_t debounceCounter);
Lab4_1State_t lab4_1SharedGetSnapshot();
void lab4_1SharedSetAppliedRelayState(bool relayOn);
void lab4_1SharedConsumeAlertCycle();

#endif
