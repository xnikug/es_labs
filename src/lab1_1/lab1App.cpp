/**
 * @file lab1App.cpp
 * @brief Implementation of the Lab 1.1 Application Logic.
 */

#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lab1App.h"
#include <ddLed.h>
#include <srvSerialStdio.h>

#define LED_PIN 13              ///< GPIO pin for the LED
#define BAUD_RATE 9600          ///< Serial baud rate
#define MAX_MESSAGE_LENGTH 10   ///< Maximum length of command string

/**
 * @brief Converts a string to lowercase in place.
 * @param str The null-terminated string to modify.
 */
static void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

/**
 * @brief Prints the list of available commands to standard output.
 */
static void printHelp() {
    printf("Available commands:\r\n");
    printf("  led on  - Turn LED on\r\n");
    printf("  led off - Turn LED off\r\n");
    printf("  help    - Show this help message\r\n\r\n");
}

/* See lab1App.h for documentation */
void lab1AppSetup() {
    srvSerialSetup(BAUD_RATE);
    
    ddLedInit(LED_PIN);
    ddLedOff(LED_PIN);
    
    printf("Lab 1: Serial LED Control\r\n");
    printf("Using STDIO for text communication\r\n\r\n");
    
    printHelp();
    
    printf("System ready. Awaiting commands...\r\n\r\n");
    printf("> ");
}

/**
 * @brief Executes logic for the 'led' command based on the modifier.
 * @param modifier The argument provided (e.g., "on" or "off").
 */
static void handleLedCommand(const char* modifier) {
    if (strcmp(modifier, "on") == 0) {
        ddLedOn(LED_PIN);
        printf("LED turned ON\r\n\r\n");
    } else if (strcmp(modifier, "off") == 0) {
        ddLedOff(LED_PIN);
        printf("LED turned OFF\r\n\r\n");
    } else {
        printf("ERROR: Unknown modifier '%s'\r\n\r\n", modifier);
    }
}

/**
 * @brief Parses and executes a complete command line.
 * @param line The null-terminated input buffer.
 */
static void lab1HandleLine(const char* line) {
    char command[MAX_MESSAGE_LENGTH] = {0};
    char modifier[MAX_MESSAGE_LENGTH] = {0};

    // Parse command and argument. sscanf returns number of items successfully matched.
    int count = sscanf(line, "%9s %9s", command, modifier);
    if (count <= 0) {
        return;
    }

    toLowerCase(command);
    toLowerCase(modifier);

    if (strcmp(command, "help") == 0) {
        printHelp();
        return;
    }

    if (strcmp(command, "led") == 0 && count >= 2) {
        handleLedCommand(modifier);
        return;
    }

    printf("ERROR: Unknown command '%s'\r\n", command);
}

/**
 * @brief Handles 'Enter' key press: executes command and resets buffer.
 * @param line The command buffer.
 * @param index Pointer to the current buffer index.
 */
static void processNewLine(char* line, size_t* index) {
    if (*index > 0) {
        line[*index] = '\0';
        putchar('\r');
        putchar('\n');
        lab1HandleLine(line);
        *index = 0;
    }
    printf("> ");
}

/**
 * @brief Handles 'Backspace' key press: removes character from buffer and terminal.
 * @param index Pointer to the current buffer index.
 */
static void processBackspace(size_t* index) {
    if (*index > 0) {
        (*index)--;
        printf("\b \b");
    }
}

/**
 * @brief Processes a standard character input.
 * @param c The character received.
 * @param line The command buffer.
 * @param index Pointer to the current buffer index.
 */
static void processChar(int c, char* line, size_t* index) {
    if (*index < MAX_MESSAGE_LENGTH - 1) {
        line[(*index)++] = (char)c;
        putchar(c);
    }
}

/* See lab1App.h for documentation */
void lab1AppLoop() {
    static char line[MAX_MESSAGE_LENGTH];
    static size_t index = 0;

    // Process incoming characters one by one to build the command line
    while (Serial.available() > 0) {
        int c = getchar();
        
        if (c == '\r' || c == '\n') {
            processNewLine(line, &index);
            continue;
        }

        if (c == '\b' || c == 127) {
            processBackspace(&index);
            continue;
        }

        processChar(c, line, &index);
    }
}
