#include "lcd_stdio.h"
#include "../../ddLcd/ddLcd.h"
#include <stdio.h>

static uint8_t s_rows    = 2U;
static uint8_t s_cur_row = 0U;
static FILE    s_lcd_file;

static int lcd_putchar(char c, FILE* stream)
{
    (void)stream;
    if (c == '\f') {
        ddLcdClear();
        ddLcdSetCursor(0, 0);
        s_cur_row = 0U;
    } else if (c == '\v') {
        ddLcdSetCursor(0, 0);
        s_cur_row = 0U;
    } else if (c == '\n') {
        s_cur_row = (uint8_t)((s_cur_row + 1U) % s_rows);
        ddLcdSetCursor(0, (int)s_cur_row);
    } else {
        ddLcdPrintChar(c);
    }
    return 0;
}

void lcd_stdio_init(uint8_t addr, uint8_t cols, uint8_t rows,
                    uint8_t sda_pin, uint8_t scl_pin)
{
    (void)addr; (void)cols; (void)sda_pin; (void)scl_pin;
    s_rows = rows;
    ddLcdInit();
    fdev_setup_stream(&s_lcd_file, lcd_putchar, NULL, _FDEV_SETUP_WRITE);
}

FILE* lcd_stdio_stream(void) { return &s_lcd_file; }
