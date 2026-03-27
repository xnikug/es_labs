/**
 * @file lab4_1SerialControl.cpp
 * @brief Serial command task for Lab 4.1.
 */

#include "lab4_1SerialControl.h"
#include "lab4_1Shared.h"
#include <stdio.h>
#include <string.h>

static bool isOnCommand(const char *command)
{
    return strcmp(command, "on") == 0;
}

static bool isOffCommand(const char *command)
{
    return strcmp(command, "off") == 0;
}

void vTask4_1SerialControl(void *pvParameters)
{
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    for (;;) {
        char token[16] = {0};
        if (scanf("%15s", token) != 1) {
            continue;
        }

        if (isOnCommand(token)) {
            lab4_1SharedSetRawCommand(false, "relay on", LAB4_1_SRC_SERIAL);
            printf("[LAB4.1] Command accepted: ON\r\n");
        } else if (isOffCommand(token)) {
            lab4_1SharedSetRawCommand(true, "relay off", LAB4_1_SRC_SERIAL);
            printf("[LAB4.1] Command accepted: OFF\r\n");
        } else {
            lab4_1SharedSetInvalidCommand(token, LAB4_1_SRC_SERIAL);
            printf("[LAB4.1] Invalid command. Use: on | off\r\n");
        }
    }
}
