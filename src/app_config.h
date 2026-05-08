#pragma once

#include <stdint.h>

/* ================================================================
 * UART
 * ================================================================ */
#define APP_UART_BAUD 9600UL

/* ================================================================
 * LCD — I2C (Wire uses hardware-fixed pins; defines are doc only)
 * ================================================================ */
#define APP_LCD_I2C_ADDR    0x27
#define APP_LCD_COLS        16
#define APP_LCD_ROWS        2
#if defined(__AVR_ATmega2560__)
  #define APP_LCD_SDA_PIN   20   /* Mega: D20 */
  #define APP_LCD_SCL_PIN   21   /* Mega: D21 */
#elif defined(__AVR_ATmega328P__)
  #define APP_LCD_SDA_PIN   18   /* Uno: A4 (SDA) */
  #define APP_LCD_SCL_PIN   19   /* Uno: A5 (SCL) */
#endif

/* ================================================================
 * Thermistor (NTC 10k) — active sensor
 *
 * Circuit: Vcc ── R_series ── A0 ── R_thermistor ── GND
 *
 * Replace NOMINAL_R / BETA with values from your specific thermistor
 * datasheet.  These defaults suit a common NTC 10k B3950 bead.
 * ================================================================ */
#define APP_THERMISTOR_PIN          A0
#define APP_THERMISTOR_SERIES_R     10000.0f  /* series resistor (Ω)          */
#define APP_THERMISTOR_NOMINAL_R    10000.0f  /* R at nominal temp (Ω)        */
#define APP_THERMISTOR_NOMINAL_T    25.0f     /* nominal temp (°C)            */
#define APP_THERMISTOR_BETA         3950.0f   /* Beta coefficient             */
#define APP_THERMISTOR_ADC_MAX      1023U     /* 10-bit ADC full scale        */
#define APP_THERMISTOR_SAMPLES      8U        /* reads averaged per sample    */

/* ================================================================
 * DHT22 Temperature & Humidity Sensor
 *   Single-wire data on D2.
 *   Passive 5V/GND resistor heats the surrounding air; DHT22
 *   measures the result.  Min 2 s between reads (DHT22 spec).
 * ================================================================ */
#define APP_DHT22_PIN           2

/* ================================================================
 * Relay — controls DC cooling fan via driver
 *   Active-LOW module: IN=LOW energises coil, IN=HIGH releases it.
 *   Relay NO contact switches DC fan power.
 * ================================================================ */
#define APP_RELAY_CONTROL_PIN   12   /* D12 */

/* ================================================================
 * Status LED — mirrors relay/fan state (D13 built-in LED)
 * ================================================================ */
#define APP_STATUS_LED_PIN      13

/* ================================================================
 * Setpoint buttons — active-LOW (INPUT_PULLUP)
 *   UP   (D4) : increase setpoint
 *   DOWN (D5) : decrease setpoint
 * ================================================================ */
#define APP_BTN_UP_PIN          4
#define APP_BTN_DOWN_PIN        5
#define APP_BTN_DEBOUNCE_MS     50U

/* Kept for button_stdio.c compilation — unused at runtime */
#define APP_BUTTON_INPUT_PIN    APP_BTN_UP_PIN
#define APP_BUTTON_DEBOUNCE_MS  APP_BTN_DEBOUNCE_MS

/* ================================================================
 * ON-OFF Hysteresis Control Parameters
 *
 * Control law:
 *   if (T > SP + HYST)  →  fan ON   (cool down)
 *   if (T < SP - HYST)  →  fan OFF  (warm up passively)
 *   else                →  maintain current state (hysteresis band)
 *
 * Passive heater (5V/GND resistor, always on) drives temperature up.
 * Fan (relay-switched) cools it down.
 * ================================================================ */
#define APP_SETPOINT_DEFAULT    25.0f   /* °C initial setpoint      */
#define APP_HYSTERESIS_DEFAULT   1.0f   /* °C half-band (±1°C band) */
#define APP_SETPOINT_STEP        0.5f   /* °C per button/key press  */
#define APP_HYSTERESIS_STEP      0.25f  /* °C per key press         */
#define APP_SETPOINT_MIN        10.0f   /* °C lower clamp           */
#define APP_SETPOINT_MAX        50.0f   /* °C upper clamp           */
#define APP_HYSTERESIS_MIN       0.25f  /* °C minimum band          */
#define APP_HYSTERESIS_MAX       5.0f   /* °C maximum band          */

/* ================================================================
 * PID Control Parameters
 * ================================================================ */
#define APP_PID_KP_DEFAULT      0.50f
#define APP_PID_KI_DEFAULT      0.05f
#define APP_PID_KD_DEFAULT      0.10f
#define APP_PID_CYCLE_MS        5000U

/* ================================================================
 * Task Periods (ms)
 * ================================================================ */
#define APP_TASK_SENSOR_MS      1000U  /* DHT22 min period: 2000 ms spec;
                                          1000 ms is safe in practice    */
#define APP_TASK_CONTROL_MS      200U  /* ON-OFF decision rate           */
#define APP_TASK_IO_MS            50U  /* Serial + button polling rate   */
#define APP_TASK_ACTUATOR_MS     100U  /* Relay drive rate               */
#define APP_TASK_DISPLAY_MS      500U  /* LCD + UART report rate         */

/* ================================================================
 * Task Stack Sizes (words; 1 word = 2 bytes on AVR)
 * Total budget: ~8 KB RAM; FreeRTOS ~1 KB; globals ~0.5 KB.
 * ================================================================ */
#define APP_STACK_SENSOR        512U   /* DHT library + dtostrf         */
#define APP_STACK_CONTROL       384U   /* PID + dtostrf + printf         */
#define APP_STACK_IO            640U   /* serial printf + button poll    */
#define APP_STACK_ACTUATOR      320U   /* relay drive                    */
#define APP_STACK_DISPLAY       512U   /* snprintf x2 + ddLcd calls      */

/* ================================================================
 * Supported boards
 * ================================================================ */
#if defined(__AVR_ATmega328P__)
  #define APP_BOARD_NAME "UNO"
#elif defined(__AVR_ATmega2560__)
  #define APP_BOARD_NAME "MEGA2560"
#else
  #error "Unsupported AVR board. Add pin mapping in app_config.h"
#endif
