#include "ddButton.h"
#include <Arduino.h>

void ddButtonInit(int pin) {
    pinMode(pin, INPUT_PULLUP);
}

int ddButtonIsPressed(int pin) {
    return digitalRead(pin) == LOW ? 1 : 0;
}