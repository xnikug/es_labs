/**
 * @file lab3_2App.cpp
 * @brief Lab 3.2 application bootstrap.
 */

#include "lab3_2App.h"
#include "lab3_2Shared.h"
#include "lab3_2Acquisition.h"
#include "lab3_2Conditioning.h"
#include "lab3_2Display.h"
#include "../srvSerialStdio/srvSerialStdio.h"
#include "../ddLcd/ddLcd.h"

void lab3_2AppSetup()
{
    srvSerialSetup(9600);

    printf("\r\n========================================\r\n");
    printf(" Lab 3.2 Signal Conditioning\r\n");
    printf("========================================\r\n");
    printf("Sensors: potentiometer(A0) + DS18B20(D5)\r\n");
    printf("Pipeline: saturation + SaltPepper + WeightedMean\r\n");
    printf("Angle: ADC->mV->deg, Temp: direct Celsius\r\n\r\n");

    lab3_2SharedInit();
    lab3_2AcquisitionInit();
    lab3_2DisplayInit();

    ddLcdClear();
    ddLcdSetCursor(0, 0);
    ddLcdSetCursor(0, 1);

    BaseType_t rc1 = xTaskCreate(vTask3_2Acquisition, "L32AngAcq", 160, NULL, 1, NULL);
    BaseType_t rc2 = xTaskCreate(vTask3_2Conditioning, "L32AngCond", 220, NULL, 1, NULL);
    BaseType_t rc3 = xTaskCreate(vTask3_2TempAcquisition, "L32TmpAcq", 320, NULL, 1, NULL);
    BaseType_t rc4 = xTaskCreate(vTask3_2TempConditioning, "L32TmpCond", 240, NULL, 1, NULL);
    BaseType_t rc5 = xTaskCreate(vTask3_2Display, "L32Disp", 340, NULL, 1, NULL);

    printf("Task create status: AA=%d AC=%d TA=%d TC=%d D=%d\r\n", rc1, rc2, rc3, rc4, rc5);
}

void lab3_2AppLoop()
{
}
