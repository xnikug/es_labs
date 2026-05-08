#include "lab5_1App.h"
#include "app_config.h"
#include "lab5_1Shared.h"
#include "lab5_1Acquisition.h"
#include "lab5_1Conditioning.h"
#include "lab5_1Control.h"
#include "lab5_1Actuation.h"
#include "lab5_1Report.h"

#include "../drivers/uart_stdio/uart_stdio.h"
#include "../drivers/led_stdio/led_stdio.h"
#include "../drivers/motor_stdio/motor_stdio.h"
#include "../drivers/lcd_stdio/lcd_stdio.h"

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <stdio.h>

void lab5_1AppSetup()
{
    uart_stdio_init(APP_UART_BAUD);
    led_stdio_init();
    motor_stdio_init(APP_RELAY_CONTROL_PIN);
    lcd_stdio_init(APP_LCD_I2C_ADDR, APP_LCD_COLS, APP_LCD_ROWS,
                   APP_LCD_SDA_PIN, APP_LCD_SCL_PIN);
    lab5_1AcquisitionInit();

    pinMode(APP_BTN_UP_PIN,   INPUT_PULLUP);
    pinMode(APP_BTN_DOWN_PIN, INPUT_PULLUP);

    lab5_1SharedInit();

    printf("\n=== LAB 5.1 ON/OFF THERMAL CONTROL ===\n");
    printf("DHT22=D%u  Relay=D%u  Btn+:D%u  Btn-:D%u\n",
           (unsigned)APP_DHT22_PIN, (unsigned)APP_RELAY_CONTROL_PIN,
           (unsigned)APP_BTN_UP_PIN, (unsigned)APP_BTN_DOWN_PIN);
    printf("--- Serial commands ---\n");
    printf("  +/-        : raise/lower setpoint by 0.5C\n");
    printf("  H/L        : raise/lower hysteresis by 0.25C\n");
    printf("  XX.X<Enter>: set setpoint directly (e.g. 26.5)\n");
    printf("  D          : toggle LCD on/off\n");
    printf("  ?          : print current status\n");
    printf("  /          : print this help\n");
    printf("---------------------------------------\n\n");

    xTaskCreate(vTask5_1Acquisition,  "L51Acq",  APP_STACK_SENSOR,   NULL, 2, NULL);
    xTaskCreate(vTask5_1Conditioning, "L51IO",   APP_STACK_IO,       NULL, 3, NULL);
    xTaskCreate(vTask5_1Control,      "L51Ctrl", APP_STACK_CONTROL,  NULL, 3, NULL);
    xTaskCreate(vTask5_1Actuation,    "L51Act",  APP_STACK_ACTUATOR, NULL, 2, NULL);
    xTaskCreate(vTask5_1Report,       "L51Rep",  APP_STACK_DISPLAY,  NULL, 1, NULL);
}

void lab5_1AppLoop()
{
}
