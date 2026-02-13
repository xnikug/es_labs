/**
 * @file ddLed.h
 * @brief LED Device Driver - Interface.
 * 
 * Provides an abstraction for controlling LED actuators.
 */

#ifndef DD_LED_H
#define DD_LED_H

/**
 * @brief Initializes the LED pin as an output.
 * @param pin The GPIO pin number connected to the LED.
 */
void ddLedInit(int pin);

/**
 * @brief Turns the LED on.
 * @param pin The GPIO pin number.
 */
void ddLedOn(int pin);

/**
 * @brief Turns the LED off.
 * @param pin The GPIO pin number.
 */
void ddLedOff(int pin);

/**
 * @brief Checks if the LED is currently on.
 * @param pin The GPIO pin number.
 * @return 1 if on, 0 otherwise.
 */
int ddLedIsOn(int pin);

/**
 * @brief Checks if the LED is currently off.
 * @param pin The GPIO pin number.
 * @return 1 if off, 0 otherwise.
 */
int ddLedIsOff(int pin);

/**
 * @brief Toggles the LED state.
 * @param pin The GPIO pin number.
 */
void ddLedToggle(int pin);

#endif