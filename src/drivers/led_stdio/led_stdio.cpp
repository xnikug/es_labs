#include "led_stdio.h"
#include "../../ddLed/ddLed.h"
#include "../../app_config.h"
#include <stdio.h>

static FILE s_led_file;

static int led_putchar(char c, FILE* stream)
{
    (void)stream;
    if (c == '1') ddLedOn(APP_STATUS_LED_PIN);
    else           ddLedOff(APP_STATUS_LED_PIN);
    return 0;
}

void led_stdio_init(void)
{
    ddLedInit(APP_STATUS_LED_PIN);
    fdev_setup_stream(&s_led_file, led_putchar, NULL, _FDEV_SETUP_WRITE);
}

FILE* led_stdio_stream(void) { return &s_led_file; }
