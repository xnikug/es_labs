/**
 * @file ddButton.h
 * @brief Button Device Driver - Interface.
 * 
 * Provides an abstraction for reading button inputs.
 */

#ifndef DD_BUTTON_H
#define DD_BUTTON_H

/**
 * @brief Initializes the button pin.
 * Configures the pin as INPUT_PULLUP.
 * @param pin The GPIO pin number connected to the button.
 */
void ddButtonInit(int pin);

/**
 * @brief Checks if the button is pressed.
 * Assumes active-low logic (grounded when pressed).
 * @param pin The GPIO pin number.
 * @return 1 if pressed, 0 otherwise.
 */
int ddButtonIsPressed(int pin);

#endif