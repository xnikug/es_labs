/**
 * @file ddLcd.h
 * @brief LCD Device Driver - Interface.
 *
 * Provides an abstraction for controlling a 16x2 I2C LCD display.
 */

#ifndef DD_LCD_H
#define DD_LCD_H

/**
 * @brief Initializes the LCD and turns on the backlight.
 */
void ddLcdInit();

/**
 * @brief Clears the LCD screen.
 */
void ddLcdClear();

/**
 * @brief Sets the cursor position.
 * @param col Column index (0-15).
 * @param row Row index (0-1).
 */
void ddLcdSetCursor(int col, int row);

/**
 * @brief Prints a string at the current cursor position.
 * @param str The null-terminated string to display.
 */
void ddLcdPrint(const char *str);

/**
 * @brief Prints a single character at the current cursor position.
 * @param c The character to display.
 */
void ddLcdPrintChar(char c);

#endif // DD_LCD_H
