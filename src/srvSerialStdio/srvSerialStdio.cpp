/**
 * @file srvSerialStdio.cpp
 * @brief Implementation of Serial Standard I/O Redirection Service.
 */

#include "Arduino.h"
#include "srvSerialStdio.h"

// Stream definitions for stdin and stdout
static FILE serialOutput = { 0 };
static FILE serialInput = { 0 };

/* See srvSerialStdio.h for documentation */
void srvSerialSetup(int baudRate) {
    Serial.begin(baudRate);
    
    while (!Serial);
    
    // Connect standard IO streams (stdin, stdout) to Serial functions
    fdev_setup_stream(&serialOutput, srvSerialWriteByte, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&serialInput, NULL, srvSerialReadByte, _FDEV_SETUP_READ);
    stdout = &serialOutput;
    stdin = &serialInput;
}

/* See srvSerialStdio.h for documentation */
int srvSerialReadByte(FILE* stream) {
    while (!Serial.available());
    return Serial.read();
}

/* See srvSerialStdio.h for documentation */
int srvSerialWriteByte(char c, FILE* stream) {
    if (c == '\n') {
        Serial.write('\r');
    }
    Serial.write(c);
    return 0;
}

/* See srvSerialStdio.h for documentation */
int srvSerialAvailable() {
    return Serial.available();
}

/* See srvSerialStdio.h for documentation */
void srvSerialFlush() {
    Serial.flush();
}

/* See srvSerialStdio.h for documentation */
void srvSerialPrintString(const char* str) {
    printf("%s", str);
}

/**
 * @brief Consumes remaining line endings (\r or \n) from the buffer.
 */
static void consumeLineEndings() {
    delay(10);
    // Swallow any extra line ending characters (\r or \n) that might remain in the buffer
    while (Serial.available()) {
        int next = Serial.peek();
        if (next == '\r' || next == '\n') {
            Serial.read();
        } else {
            break;
        }
    }
}

/* See srvSerialStdio.h for documentation */
int srvSerialReadLine(char* buffer, int maxLength) {
    int index = 0;
    
    while (index < maxLength - 1) {
        while (!Serial.available());
        
        int c = Serial.read();
        
        if (c == '\r' || c == '\n') {
            consumeLineEndings();
            break;
        }
        
        buffer[index++] = (char)c;
    }
    
    buffer[index] = '\0';
    return index;
}