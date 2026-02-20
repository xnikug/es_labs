/**
 * @file lab1_2App.h
 * @brief Application interface for Lab 1.2 - LCD + Keypad with STDIO.
 *
 * Configures stdout -> LCD and stdin -> Keypad via srvLcdStdio, then
 * runs a passcode entry application:
 *   - printf() writes prompts and results to the LCD display.
 *   - scanf() / getchar() reads key presses from the 4x4 keypad.
 *   - Green LED lights on valid code, Red LED lights on invalid code.
 */

#ifndef LAB_1_2_APP_H
#define LAB_1_2_APP_H

/**
 * @brief Initializes all peripherals and the LCD+Keypad STDIO streams.
 */
void lab1_2AppSetup();

/**
 * @brief Main loop: reads a 4-character code via scanf and validates it.
 */
void lab1_2AppLoop();

#endif // LAB_1_2_APP_H