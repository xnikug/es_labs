/**
 * @file ddKeyPad.cpp
 * @brief Keypad Device Driver - Implementation.
 */

#include "ddKeyPad.h"
#include <Keypad.h>

#define DD_KEYPAD_REPEAT_DELAY  100
#define DD_KEYPAD_ROW_PIN_0     22
#define DD_KEYPAD_ROW_PIN_1     24
#define DD_KEYPAD_ROW_PIN_2     26
#define DD_KEYPAD_ROW_PIN_3     28
#define DD_KEYPAD_COL_PIN_0     30
#define DD_KEYPAD_COL_PIN_1     32
#define DD_KEYPAD_COL_PIN_2     34
#define DD_KEYPAD_COL_PIN_3     36

static const byte ROWS = 4;
static const byte COLS = 4;

static char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

static byte rowPins[ROWS] = {
    DD_KEYPAD_ROW_PIN_0, DD_KEYPAD_ROW_PIN_1,
    DD_KEYPAD_ROW_PIN_2, DD_KEYPAD_ROW_PIN_3
};
static byte colPins[COLS] = {
    DD_KEYPAD_COL_PIN_0, DD_KEYPAD_COL_PIN_1,
    DD_KEYPAD_COL_PIN_2, DD_KEYPAD_COL_PIN_3
};

static Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/* See ddKeyPad.h for documentation */
void ddKeyPadInit()
{
    // Keypad library handles pin setup internally
}

/* See ddKeyPad.h for documentation */
char ddKeyPadGetKey()
{
    char key;
    do {
        key = keypad.getKey();
    } while (key == NO_KEY);

    delay(DD_KEYPAD_REPEAT_DELAY);
    return key;
}

/* See ddKeyPad.h for documentation */
char ddKeyPadRead()
{
    return keypad.getKey();
}