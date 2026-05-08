/**
 * @file edRelay.h
 * @brief Relay equipment driver - interface.
 */

#ifndef ED_RELAY_H
#define ED_RELAY_H

#include <Arduino.h>
#include "../app_config.h"

#ifdef APP_RELAY_CONTROL_PIN
#define ED_RELAY_PIN APP_RELAY_CONTROL_PIN
#else
#define ED_RELAY_PIN 6
#endif

#ifndef ED_RELAY_ACTIVE_LEVEL
#define ED_RELAY_ACTIVE_LEVEL LOW
#endif

#ifndef ED_RELAY_INACTIVE_LEVEL
#define ED_RELAY_INACTIVE_LEVEL HIGH
#endif

void edRelayInit();
void edRelayOn();
void edRelayOff();
bool edRelayIsOn();

#endif
