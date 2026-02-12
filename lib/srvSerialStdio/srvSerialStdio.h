#ifndef SRV_SERIAL_STDIO_H
#define SRV_SERIAL_STDIO_H

#include <stdio.h>
#include <Arduino.h>

void srvSerialSetup(int baudRate);

int srvSerialReadByte(FILE* stream);
int srvSerialWriteByte(char c, FILE* stream);

int srvSerialAvailable();

void srvSerialFlush();

void srvSerialPrintString(const char* str);

int srvSerialReadLine(char* buffer, int maxLength);

#endif