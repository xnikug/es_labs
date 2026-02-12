#include "ddLed.h"
#include <Arduino.h>

void ddLedInit(int pin) {
    pinMode(pin, OUTPUT);
}

void ddLedOn(int pin) {
    digitalWrite(pin, HIGH);
}

void ddLedOff(int pin) {
    digitalWrite(pin, LOW);
}

int ddLedIsOn(int pin) {
    return digitalRead(pin) == HIGH ? 1 : 0;
}
int ddLedIsOff(int pin) {
    return digitalRead(pin) == LOW ? 1 : 0;
}

void ddLedToggle(int pin) {
    if(ddLedIsOn(pin)) {
        ddLedOff(pin);
    } else {
        ddLedOn(pin);
    }
}