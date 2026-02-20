#include "lab1_2App.h"
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "srvLcdStdio/srvLcdStdio.h"
#include "ddLed/ddLed.h"

#define LED_RED_PIN      11
#define LED_GREEN_PIN    12
#define MAX_CODE_LENGTH  4
#define VALID_CODE       "708#"
#define RESULT_DELAY_MS  2000

static void passcodeInit()
{
    printf("\fEnter code:\n");
}

static void validatePasscode(const char* code)
{
    printf("\f");
    if (strcmp(code, VALID_CODE) == 0) {
        printf("Code Valid!");
        ddLedOn(LED_GREEN_PIN);
        ddLedOff(LED_RED_PIN);
    } else {
        printf("Invalid Code!");
        ddLedOff(LED_GREEN_PIN);
        ddLedOn(LED_RED_PIN);
    }

    delay(RESULT_DELAY_MS);

    ddLedOff(LED_GREEN_PIN);
    ddLedOff(LED_RED_PIN);
    passcodeInit();
}

void lab1_2AppSetup()
{
    ddLedInit(LED_RED_PIN);
    ddLedInit(LED_GREEN_PIN);
    ddLedOff(LED_RED_PIN);
    ddLedOff(LED_GREEN_PIN);
    srvLcdStdioSetup();
    passcodeInit();
}

void lab1_2AppLoop()
{
    char enteredCode[MAX_CODE_LENGTH + 1];

    // Read exactly 4 characters from stdin (keypad) via scanf
    scanf("%4s", enteredCode);

    validatePasscode(enteredCode);
}
