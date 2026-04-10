#include "lab4_2App.h"

#include "lab4_2Shared.h"
#include "lab4_2Control.h"
#include "lab4_2SerialInput.h"
#include "lab4_2Report.h"
#include "lab4_2Alert.h"

#include "../ddLcd/ddLcd.h"
#include "../srvSerialStdio/srvSerialStdio.h"

#include <Arduino_FreeRTOS.h>
#include <stdio.h>

static void lab4_2PrintIntroAndCaptureModes()
{
    printf("\r\n=== L42 DRIVE CONSOLE ===\r\n");
    gLab4_2InputSource = 1;
    printf("SRC fixed=UART (no potentiometer)\r\n");

    printf("VIEW [0=STREAM, 1=PANEL] -> ");
    fflush(stdout);

    int outputChoice = -1;
    scanf("%d", &outputChoice);
    if (outputChoice < 0 || outputChoice > 1) {
        outputChoice = 0;
    }
    gLab4_2OutputMode = (uint8_t)outputChoice;

    printf("CONFIG src=UART view=%u\r\n", (unsigned)gLab4_2OutputMode);
    printf("UART CMD: send signed setpoint in range [-100..100]\r\n");
}

void lab4_2AppSetup()
{
    srvSerialSetup(9600);

    lab4_2SharedInit();
    lab4_2PrintIntroAndCaptureModes();

    if (gLab4_2OutputMode == 1) {
        ddLcdInit();
        ddLcdClear();
    }

    gLab4_2PowerMutex = xSemaphoreCreateMutex();

    BaseType_t rcControl = xTaskCreate(vTask4_2Control, "L42Ctrl", 360, NULL, 1, NULL);
    BaseType_t rcInput = xTaskCreate(vTask4_2SerialInput, "L42Input", 320, NULL, 1, NULL);
    BaseType_t rcReport = xTaskCreate(vTask4_2Report, "L42Report", 380, NULL, 1, NULL);
    BaseType_t rcAlert = xTaskCreate(vTask4_2Alert, "L42Alert", 220, NULL, 1, NULL);

    printf("TASKS ctrl=%d io=%d view=%d alert=%d\r\n", rcControl, rcInput, rcReport, rcAlert);
}

void lab4_2AppLoop()
{
}
