#include "lab5_2App.h"
#include "app_config.h"
#include "lab5_2Shared.h"
#include "lab5_2Acquisition.h"
#include "lab5_2Control.h"
#include "lab5_2Output.h"
#include "lab5_2Report.h"

#include "../drivers/uart_stdio/uart_stdio.h"
#include "../drivers/led_stdio/led_stdio.h"
#include "../drivers/motor_stdio/motor_stdio.h"
#include "../drivers/lcd_stdio/lcd_stdio.h"

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <stdio.h>

void lab5_2AppSetup()
{
    uart_stdio_init(APP_UART_BAUD);
    led_stdio_init();
    motor_stdio_init(APP_RELAY_CONTROL_PIN);
    lcd_stdio_init(APP_LCD_I2C_ADDR, APP_LCD_COLS, APP_LCD_ROWS,
                   APP_LCD_SDA_PIN, APP_LCD_SCL_PIN);
    lab5_2AcquisitionInit();

    pinMode(APP_BTN_UP_PIN,   INPUT_PULLUP);
    pinMode(APP_BTN_DOWN_PIN, INPUT_PULLUP);

    lab5_2SharedInit();

#if LAB5_2_OUTPUT_MODE == LAB5_2_MODE_MONITOR
    printf("\n=== LAB 5.2 PID THERMAL CONTROL ===\n");
    printf("DHT22=D%u  Relay=D%u  Btn+:D%u  Btn-:D%u\n",
           (unsigned)APP_DHT22_PIN, (unsigned)APP_RELAY_CONTROL_PIN,
           (unsigned)APP_BTN_UP_PIN, (unsigned)APP_BTN_DOWN_PIN);
    printf("--- Serial commands ---\n");
    printf("  +/-        : raise/lower setpoint by 0.5C\n");
    printf("  XX.X<Enter>: set setpoint directly (e.g. 26.5)\n");
    printf("  p<Enter>   : enter new Kp gain\n");
    printf("  i<Enter>   : enter new Ki gain\n");
    printf("  k<Enter>   : enter new Kd gain\n");
    printf("  D          : toggle LCD on/off\n");
    printf("  ?          : print current status\n");
    printf("  /          : print this help\n");
    printf("---------------------------------------\n\n");
#endif

    xTaskCreate(vTask5_2Acquisition, "L52Acq",  APP_STACK_SENSOR,   NULL, 2, NULL);
    xTaskCreate(vTask5_2Control,     "L52Ctrl", APP_STACK_IO,       NULL, 3, NULL);
    xTaskCreate(vTask5_2Output,      "L52Out",  APP_STACK_ACTUATOR, NULL, 2, NULL);
    xTaskCreate(vTask5_2Report,      "L52Rep",  APP_STACK_DISPLAY,  NULL, 1, NULL);
}

void lab5_2AppLoop()
{
}
