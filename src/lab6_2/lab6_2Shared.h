#pragma once
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

typedef enum {
    TL_EAST_GREEN   = 0,
    TL_EAST_YELLOW  = 1,
    TL_ALL_RED_A    = 2,
    TL_NORTH_GREEN  = 3,
    TL_NORTH_YELLOW = 4,
    TL_ALL_RED_B    = 5,
    TL_STATE_COUNT  = 6
} TlState_t;

extern SemaphoreHandle_t g_northReqSem;

void      lab6_2SharedInit(void);
void      lab6_2SharedSetState(TlState_t s);
TlState_t lab6_2SharedGetState(void);
