/**
 * @file ddLed.cpp
 * @brief LED Device Driver - Implementation.
 */

#include "ddLed.h"
#include <Arduino.h>

/* See ddLed.h for documentation */
void ddLedInit(int pin) {
    pinMode(pin, OUTPUT);
}

/* See ddLed.h for documentation */
void ddLedOn(int pin) {
    digitalWrite(pin, HIGH);
}

/* See ddLed.h for documentation */
void ddLedOff(int pin) {
    digitalWrite(pin, LOW);
}

/* See ddLed.h for documentation */
int ddLedIsOn(int pin) {
    return digitalRead(pin) == HIGH ? 1 : 0;
}

/* See ddLed.h for documentation */
int ddLedIsOff(int pin) {
    return digitalRead(pin) == LOW ? 1 : 0;
}

/* See ddLed.h for documentation */
void ddLedToggle(int pin) {
    if(ddLedIsOn(pin)) {
        ddLedOff(pin);
    } else {
        ddLedOn(pin);
    }
}