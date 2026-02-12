#ifndef DD_LED_H
#define DD_LED_H

void ddLedInit(int pin);
void ddLedOn(int pin);
void ddLedOff(int pin);
int ddLedIsOn(int pin);
int ddLedIsOff(int pin);
void ddLedToggle(int pin);

#endif