/**
 * @file lab3_1App.cpp
 * @brief Lab 3.1 application bootstrap.
 */

#include "lab3_1App.h"
#include "lab3_1Shared.h"
#include "lab3_1Acquisition.h"
#include "lab3_1Conditioning.h"
#include "lab3_1Display.h"
#include "../srvSerialStdio/srvSerialStdio.h"
#include "../ddLcd/ddLcd.h"

/**
 * @brief Performs complete system startup for Lab 3.1.
 *
 * Startup sequence:
 * 1) Serial setup and startup banner.
 * 2) Shared synchronization objects creation.
 * 3) Sensor/device initialization.
 * 4) LCD initialization.
 * 5) Task creation for acquisition, conditioning, alerting, and reporting.
 */
void lab3_1AppSetup() {
    srvSerialSetup(9600);

    printf("\r\n========================================\r\n");
    printf(" Lab 3.1 Angle + Temperature Monitor\r\n");
    printf("========================================\r\n");
    printf("Angle source: potentiometer (0..270 deg)\r\n");
    printf("Angle center offset: %d deg\r\n", LAB3_1_ANGLE_OFFSET_DEG);
    printf("Angle window: [%d .. %d] deg\r\n", LAB3_1_ANGLE_LOW_THRESHOLD_DEG, LAB3_1_ANGLE_HIGH_THRESHOLD_DEG);
    printf("Debounce max count: %d\r\n\r\n", LAB3_1_ANGLE_DEBOUNCE_MAX_COUNT);

    lab3_1SharedInit();
    lab3_1AcquisitionInit();
    lab3_1DisplayInit();

    ddLcdClear();
    ddLcdSetCursor(0, 0);
    ddLcdSetCursor(0, 1);

    // Task creation status values are printed for quick startup diagnostics.
    BaseType_t rc1 = xTaskCreate(vTaskAngleAcquisition, "AngleAcq", 160, NULL, 1, NULL);
    BaseType_t rc2 = xTaskCreate(vTaskDisplay, "Report", 320, NULL, 1, NULL);
    BaseType_t rc3 = xTaskCreate(vTaskTempAcquisition, "TempAcq", 320, NULL, 1, NULL);
    BaseType_t rc4 = xTaskCreate(vTaskConditioning, "AngleCond", 160, NULL, 1, NULL);
    BaseType_t rc5 = xTaskCreate(vTaskAngleAlert, "AngleAlert", 160, NULL, 1, NULL);

    printf("Task create status: A=%d D=%d T=%d C=%d L=%d\r\n", rc1, rc2, rc3, rc4, rc5);
}

/**
 * @brief Empty loop for Arduino compatibility.
 */
void lab3_1AppLoop() {}

