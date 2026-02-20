/**
 * @file srvLcdStdio.cpp
 * @brief LCD + Keypad Standard I/O Redirection Service - Implementation.
 */

#include "srvLcdStdio.h"
#include "ddLcd/ddLcd.h"
#include "ddKeyPad/ddKeyPad.h"

#define LCD_NEWLINE_COL  0
#define LCD_NEWLINE_ROW  1

static FILE lcdOutput = { 0 };
static FILE keypadInput = { 0 };

/* See srvLcdStdio.h for documentation */
int srvLcdWriteByte(char c, FILE* stream)
{
    if (c == '\f') {
        // Form-feed: clear display and home cursor
        ddLcdClear();
        ddLcdSetCursor(0, 0);
    } else if (c == '\n') {
        // Newline: move to the second row
        ddLcdSetCursor(LCD_NEWLINE_COL, LCD_NEWLINE_ROW);
    } else {
        ddLcdPrintChar(c);
    }
    return 0;
}

/* See srvLcdStdio.h for documentation */
int srvLcdReadByte(FILE* stream)
{
    return (int)ddKeyPadGetKey();
}

/* See srvLcdStdio.h for documentation */
void srvLcdStdioSetup()
{
    ddLcdInit();
    ddKeyPadInit();

    fdev_setup_stream(&lcdOutput,  srvLcdWriteByte, NULL,             _FDEV_SETUP_WRITE);
    fdev_setup_stream(&keypadInput, NULL,           srvLcdReadByte,   _FDEV_SETUP_READ);

    stdout = &lcdOutput;
    stdin  = &keypadInput;
}
