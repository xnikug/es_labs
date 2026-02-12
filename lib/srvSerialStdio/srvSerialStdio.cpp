#include "Arduino.h"
#include "srvSerialStdio.h"

static FILE serialOutput = { 0 };
static FILE serialInput = { 0 };

void srvSerialSetup(int baudRate) {
    Serial.begin(baudRate);
    
    while (!Serial);
    
    // Connect standard IO streams (stdin, stdout) to Serial functions
    fdev_setup_stream(&serialOutput, srvSerialWriteByte, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&serialInput, NULL, srvSerialReadByte, _FDEV_SETUP_READ);
    stdout = &serialOutput;
    stdin = &serialInput;
}

int srvSerialReadByte(FILE* stream) {
    while (!Serial.available());
    return Serial.read();
}

int srvSerialWriteByte(char c, FILE* stream) {
    Serial.write(c);
    return 0;
}

int srvSerialAvailable() {
    return Serial.available();
}

void srvSerialFlush() {
    Serial.flush();
}

void srvSerialPrintString(const char* str) {
    printf("%s", str);
}

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