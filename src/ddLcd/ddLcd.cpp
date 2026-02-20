/**
 * @file ddLcd.cpp
 * @brief LCD Device Driver - Implementation.
 */

#include "ddLcd.h"
#include <LiquidCrystal_I2C.h>

#define LCD_I2C_ADDRESS 0x27
#define LCD_COLUMNS     16
#define LCD_ROWS        2

// SDA = pin 20, SCL = pin 21 on Arduino Mega.
static LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

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
