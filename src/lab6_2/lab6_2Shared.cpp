#include "lab6_2Shared.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

SemaphoreHandle_t g_northReqSem = NULL;

static volatile TlState_t g_trafficState = TL_EAST_GREEN;

void lab6_2SharedInit(void) {
    g_northReqSem  = xSemaphoreCreateBinary();
    g_trafficState = TL_EAST_GREEN;
}

void lab6_2SharedSetState(TlState_t s) {
    taskENTER_CRITICAL();
    g_trafficState = s;
    taskEXIT_CRITICAL();
}

TlState_t lab6_2SharedGetState(void) {
    TlState_t s;
    taskENTER_CRITICAL();
    s = g_trafficState;
    taskEXIT_CRITICAL();
    return s;
}
