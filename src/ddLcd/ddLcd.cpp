/**
 * @file ddLcd.cpp
 * @brief LCD Device Driver - Implementation.
 */

#include "ddLcd.h"
#include "app_config.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// SDA = pin 20, SCL = pin 21 on Arduino Mega. (hardware fixed)
static LiquidCrystal_I2C lcd(APP_LCD_I2C_ADDR, APP_LCD_COLS, APP_LCD_ROWS);

/* See ddLcd.h for documentation */
void ddLcdInit()
{
    lcd.init();
    lcd.backlight();
}

/* See ddLcd.h for documentation */
void ddLcdClear()
{
    lcd.clear();
}

/* See ddLcd.h for documentation */
void ddLcdSetCursor(int col, int row)
{
    lcd.setCursor(col, row);
}

/* See ddLcd.h for documentation */
void ddLcdPrint(const char *str)
{
    lcd.print(str);
}

/* See ddLcd.h for documentation */
void ddLcdPrintChar(char c)
{
    lcd.print(c);
}
