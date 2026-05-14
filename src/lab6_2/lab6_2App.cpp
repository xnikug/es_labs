#include "lab6_2App.h"
#include "lab6_2Shared.h"
#include "lab6_2Fsm.h"
#include "lab6_2Traffic.h"
#include "lab6_2Button.h"
#include "lab6_2Report.h"
#include "../app_config.h"
#include "../drivers/uart_stdio/uart_stdio.h"
#include <Arduino_FreeRTOS.h>

void lab6_2AppSetup(void) {
    uart_stdio_init(APP_UART_BAUD);
    lab6_2FsmInitPins();
    lab6_2SharedInit();

    xTaskCreate(vTask6_2Button,  "L62Btn", APP_STACK_LAB6_2_BUTTON,  NULL, 4, NULL);
    xTaskCreate(vTask6_2Traffic, "L62TL",  APP_STACK_LAB6_2_TRAFFIC, NULL, 3, NULL);
    xTaskCreate(vTask6_2Report,  "L62Rep", APP_STACK_LAB6_2_REPORT,  NULL, 1, NULL);
}

void lab6_2AppLoop(void) {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
