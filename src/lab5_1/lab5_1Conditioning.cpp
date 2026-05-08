#include "lab5_1Conditioning.h"
#include "lab5_1Shared.h"
#include "../drivers/uart_stdio/uart_stdio.h"

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

static uint8_t  s_up_last = 1U,  s_dn_last = 1U;
static uint32_t s_up_time = 0UL, s_dn_time = 0UL;
static char     s_buf[10];
static uint8_t  s_idx = 0U;

void vTask5_1Conditioning(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        uint32_t now = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);

        uint8_t up = (uint8_t)digitalRead(APP_BTN_UP_PIN);
        if (up == 0U && s_up_last == 1U &&
            (uint32_t)(now - s_up_time) >= (uint32_t)APP_BTN_DEBOUNCE_MS)
        {
            s_up_time = now;
            control_state_t ctrl; app_shared_get_control(&ctrl);
            float sp = ctrl.setpoint_c + APP_SETPOINT_STEP;
            if (sp > APP_SETPOINT_MAX) sp = APP_SETPOINT_MAX;
            app_shared_set_setpoint(sp);
        }
        s_up_last = up;

        uint8_t dn = (uint8_t)digitalRead(APP_BTN_DOWN_PIN);
        if (dn == 0U && s_dn_last == 1U &&
            (uint32_t)(now - s_dn_time) >= (uint32_t)APP_BTN_DEBOUNCE_MS)
        {
            s_dn_time = now;
            control_state_t ctrl; app_shared_get_control(&ctrl);
            float sp = ctrl.setpoint_c - APP_SETPOINT_STEP;
            if (sp < APP_SETPOINT_MIN) sp = APP_SETPOINT_MIN;
            app_shared_set_setpoint(sp);
        }
        s_dn_last = dn;

        int ch = uart_getchar_nb();
        if (ch != EOF) {
            if (ch == '+') {
                s_idx = 0U;
                control_state_t ctrl; app_shared_get_control(&ctrl);
                float sp = ctrl.setpoint_c + APP_SETPOINT_STEP;
                if (sp > APP_SETPOINT_MAX) sp = APP_SETPOINT_MAX;
                app_shared_set_setpoint(sp);
            } else if (ch == '-') {
                s_idx = 0U;
                control_state_t ctrl; app_shared_get_control(&ctrl);
                float sp = ctrl.setpoint_c - APP_SETPOINT_STEP;
                if (sp < APP_SETPOINT_MIN) sp = APP_SETPOINT_MIN;
                app_shared_set_setpoint(sp);
            } else if (ch == 'H' || ch == 'h') {
                s_idx = 0U;
                control_state_t ctrl; app_shared_get_control(&ctrl);
                float h = ctrl.hysteresis_c + APP_HYSTERESIS_STEP;
                if (h > APP_HYSTERESIS_MAX) h = APP_HYSTERESIS_MAX;
                app_shared_set_hysteresis(h);
            } else if (ch == 'L' || ch == 'l') {
                s_idx = 0U;
                control_state_t ctrl; app_shared_get_control(&ctrl);
                float h = ctrl.hysteresis_c - APP_HYSTERESIS_STEP;
                if (h < APP_HYSTERESIS_MIN) h = APP_HYSTERESIS_MIN;
                app_shared_set_hysteresis(h);
            } else if (ch == 'D' || ch == 'd') {
                s_idx = 0U;
                app_shared_toggle_display();
            } else if (ch == '?') {
                s_idx = 0U;
                control_state_t ctrl; sensor_data_t sensor; relay_state_t relay;
                app_shared_get_control(&ctrl);
                app_shared_get_sensor(&sensor);
                app_shared_get_relay(&relay);
                char sp[8], hyst[8], t[8];
                dtostrf(ctrl.setpoint_c,      5, 1, sp);
                dtostrf(ctrl.hysteresis_c,    4, 2, hyst);
                dtostrf(sensor.temperature_c, 5, 1, t);
                printf("SP=%sC T=%sC HYST=%sC FAN=%s valid=%u\n",
                       sp, t, hyst,
                       relay.is_on ? "ON" : "OFF",
                       (unsigned)sensor.valid);
            } else if (ch == '/') {
                s_idx = 0U;
                printf("--- Serial commands ---\n");
                printf("  +/-        : raise/lower setpoint by 0.5C\n");
                printf("  H/L        : raise/lower hysteresis by 0.25C\n");
                printf("  XX.X<Enter>: set setpoint directly (e.g. 26.5)\n");
                printf("  D          : toggle LCD on/off\n");
                printf("  ?          : print current status\n");
                printf("  /          : print this help\n");
            } else if (ch == '\r' || ch == '\n') {
                if (s_idx > 0U) {
                    s_buf[s_idx] = '\0'; s_idx = 0U;
                    float val = (float)atof(s_buf);
                    if (val >= APP_SETPOINT_MIN && val <= APP_SETPOINT_MAX)
                        app_shared_set_setpoint(val);
                }
            } else if ((ch >= '0' && ch <= '9') || ch == '.') {
                if (s_idx < (uint8_t)(sizeof(s_buf) - 1U))
                    s_buf[s_idx++] = (char)ch;
            } else {
                s_idx = 0U;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_1_TASK_COND_MS));
    }
}
