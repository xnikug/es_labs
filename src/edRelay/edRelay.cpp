/**
 * @file edRelay.cpp
 * @brief Relay equipment driver - implementation.
 */

#include "edRelay.h"

static bool gEdRelayOn = false;

void edRelayInit()
{
    pinMode(ED_RELAY_PIN, OUTPUT);
    digitalWrite(ED_RELAY_PIN, ED_RELAY_INACTIVE_LEVEL);
    gEdRelayOn = false;
}

void edRelayOn()
{
    digitalWrite(ED_RELAY_PIN, ED_RELAY_ACTIVE_LEVEL);
    gEdRelayOn = true;
}

void edRelayOff()
{
    digitalWrite(ED_RELAY_PIN, ED_RELAY_INACTIVE_LEVEL);
    gEdRelayOn = false;
}

bool edRelayIsOn()
{
    return gEdRelayOn;
}
