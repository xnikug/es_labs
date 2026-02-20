/**
 * @file srvLcdStdio.h
 * @brief LCD + Keypad Standard I/O Redirection Service - Interface.
 *
 * Redirects the standard C I/O streams so that:
 *   - stdout  →  LCD display  (printf / putchar write to the LCD)
 *   - stdin   →  Keypad       (scanf  / getchar read from the keypad)
 *
 * Usage:
 *   srvLcdStdioSetup();          // call once in setup()
 *   printf("Hello!");            // appears on the LCD
 *   char c = getchar();          // blocks until a key is pressed
 */

#ifndef SRV_LCD_STDIO_H
#define SRV_LCD_STDIO_H

#include <stdio.h>

/**
 * @brief Initializes the LCD and keypad drivers and binds them to
 *        stdout / stdin via fdev_setup_stream.
 */
void srvLcdStdioSetup();

/**
 * @brief Low-level write function used by fdev_setup_stream.
 *        Interprets '\n' as a cursor move to column 0 of row 1,
 *        and '\f' (form-feed) as a full display clear.
 * @param c      Character to write.
 * @param stream File stream pointer (unused).
 * @return 0 on success.
 */
int srvLcdWriteByte(char c, FILE* stream);

/**
 * @brief Low-level read function used by fdev_setup_stream.
 *        Blocks until a key is pressed on the keypad.
 * @param stream File stream pointer (unused).
 * @return The ASCII value of the key pressed.
 */
int srvLcdReadByte(FILE* stream);

#endif // SRV_LCD_STDIO_H
