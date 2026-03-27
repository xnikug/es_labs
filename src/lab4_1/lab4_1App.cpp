/**
 * @file lab4_1App.cpp
 * @brief Lab 4.1 application bootstrap.
 */

#include "lab4_1App.h"
#include "lab4_1Shared.h"
#include "lab4_1SerialControl.h"
#include "lab4_1Conditioning.h"
#include "lab4_1Actuation.h"
#include "lab4_1Report.h"
#include "../srvSerialStdio/srvSerialStdio.h"
#include "../ddLcd/ddLcd.h"
#include "../ddFan/ddFan.h"
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

void lab4_1AppSetup()
{
    srvSerialSetup(9600);
    ddLcdInit();
    ddFanInit();
    lab4_1SharedInit();

    printf("\r\n\r\nSYSTEM CONFIGURATION:\r\n");
    printf("-------------------------------------\r\n");
    printf("Lab: 4.1 - Binary Actuator Fan\r\n\r\n");
    printf("Binary Actuator: Fan\r\n");
    printf("  - Commands:           on / off\r\n");
    printf("  - Debounce Max:       %d cycles\r\n", LAB4_1_DEBOUNCE_MAX_COUNTER);
    printf("  - Cond. Recurrence:   100 ms\r\n");
    printf("  - Debounce Time:      %d ms\r\n\r\n", LAB4_1_DEBOUNCE_MAX_COUNTER * 100);
    printf("Saturation:\r\n");
    printf("  - Valid inputs:       on/off or relay on/off\r\n");
    printf("  - Invalid input:      rejected, raw command unchanged\r\n\r\n");
    printf("Display:\r\n");
    printf("  - Serial report:      500 ms\r\n");
    printf("  - LCD update:         500 ms\r\n\r\n");
    printf("System Status: STARTING\r\n");
    printf("-------------------------------------\r\n\r\n");
    printf("Type 'on' or 'off' and press Enter\r\n\r\n");

    ddLcdClear();
    ddLcdSetCursor(0, 0);
    ddLcdPrint("Lab4.1 Fan");
    ddLcdSetCursor(0, 1);
    ddLcdPrint("Type on/off");

    BaseType_t rc1 = xTaskCreate(vTask4_1SerialControl, "L41Serial", 260, NULL, 1, NULL);
    BaseType_t rc2 = xTaskCreate(vTask4_1Conditioning, "L41Cond", 220, NULL, 1, NULL);
    BaseType_t rc3 = xTaskCreate(vTask4_1Actuation, "L41Fan", 180, NULL, 1, NULL);
    BaseType_t rc4 = xTaskCreate(vTask4_1Report, "L41Report", 300, NULL, 1, NULL);

    printf("Task create status: Serial=%d Cond=%d Act=%d Report=%d\r\n", rc1, rc2, rc3, rc4);
}

void lab4_1AppLoop()
{
}
