/**
 * @file ddFan.h
 * @brief Fan device driver - interface.
 */

#ifndef DD_FAN_H
#define DD_FAN_H

#include <Arduino.h>

void ddFanInit();
void ddFanOn();
void ddFanOff();
bool ddFanIsOn();

#endif
