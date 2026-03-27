/**
 * @file ddFan.cpp
 * @brief Fan device driver - implementation.
 */

#include "ddFan.h"
#include "../edRelay/edRelay.h"

static bool gDdFanOn = false;

void ddFanInit()
{
    edRelayInit();
    gDdFanOn = false;
}

void ddFanOn()
{
    edRelayOn();
    gDdFanOn = true;
}

void ddFanOff()
{
    edRelayOff();
    gDdFanOn = false;
}

bool ddFanIsOn()
{
    return gDdFanOn;
}
