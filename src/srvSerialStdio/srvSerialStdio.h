/**
 * @file srvSerialStdio.h
 * @brief Serial Standard I/O Redirection Service.
 * 
 * Abstract service that redirects standard C input/output streams (stdin, stdout)
 * to the microcontroller's UART interface.
 */

#ifndef SRV_SERIAL_STDIO_H
#define SRV_SERIAL_STDIO_H

#include <stdio.h>
#include <Arduino.h>

/**
 * @brief Initializes the serial service and binds stdio streams.
 * 
 * @param baudRate The communication speed in bits per second.
 */
void srvSerialSetup(int baudRate);

/**
 * @brief Low-level read function used by fdev_setup_stream.
 * Blocking until a character is available.
 * @param stream File stream pointer.
 * @return The character read.
 */
int srvSerialReadByte(FILE* stream);

/**
 * @brief Low-level write function used by fdev_setup_stream.
 * @param c The character to write.
 * @param stream File stream pointer.
 * @return 0 on success.
 */
int srvSerialWriteByte(char c, FILE* stream);

/**
 * @brief Checks if data is available in the hardware serial buffer.
 * @return The number of bytes available.
 */
int srvSerialAvailable();

/**
 * @brief Flushes the hardware serial transmission buffer.
 */
void srvSerialFlush();

/**
 * @brief Helper to print a string directly to serial (wraps printf).
 * @param str The null-terminated string to print.
 */
void srvSerialPrintString(const char* str);

/**
 * @brief Reads a line from serial until newline or buffer full.
 * @param buffer Destination buffer.
 * @param maxLength Maximum characters to read.
 * @return Number of characters read.
 */
int srvSerialReadLine(char* buffer, int maxLength);

#endif