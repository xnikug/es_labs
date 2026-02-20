/**
 * @file ddKeyPad.h
 * @brief Keypad Device Driver - Interface.
 *
 * Provides an abstraction for reading input from a 4x4 matrix keypad.
 */

#ifndef DD_KEYPAD_H
#define DD_KEYPAD_H

/**
 * @brief Initializes the keypad driver.
 */
void ddKeyPadInit();

/**
 * @brief Waits for and returns the next key press.
 * @return The character of the key pressed.
 */
char ddKeyPadGetKey();

/**
 * @brief Returns the key currently pressed, or 0 if none.
 * @return The character of the key pressed, or 0 (NO_KEY).
 */
char ddKeyPadRead();

#endif // DD_KEYPAD_H