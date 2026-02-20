/**
 * @file ddButton.cpp
 * @brief Button Device Driver - Implementation.
 */

#include "ddButton.h"
#include <Arduino.h>

/* See ddButton.h for documentation */
void ddButtonInit(int pin) {
    pinMode(pin, INPUT_PULLUP);
}

/* See ddButton.h for documentation */
int ddButtonIsPressed(int pin) {
    return digitalRead(pin) == LOW ? 1 : 0;
}