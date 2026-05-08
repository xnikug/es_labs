#include "lab5_2Control.h"
#include "lab5_2Shared.h"
#include "../control/pid_control.h"
#include "../drivers/uart_stdio/uart_stdio.h"

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum { INPUT_SP = 0, INPUT_KP, INPUT_KI, INPUT_KD } input_target_t;

static uint8_t       s_up_last  = 1U,  s_dn_last  = 1U;
static uint32_t      s_up_time  = 0UL, s_dn_time  = 0UL;
static input_target_t s_target  = INPUT_SP;
static char          s_buf[10];
static uint8_t       s_idx      = 0U;

void vTask5_2Control(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const float dt = (float)LAB5_2_TASK_CTRL_MS / 1000.0f;

    pid_state_t pid;
    pid_params_t p;
    app_shared_get_pid_params(&p);
    pid_init(&pid, p.kp, p.ki, p.kd, dt, 0.0f, 1.0f);

    for (;;) {
        uint32_t now = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);

        /* buttons */
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

        /* serial */
        int ch = uart_getchar_nb();
        if (ch != EOF) {
            if (ch == '+') {
                s_idx = 0U; s_target = INPUT_SP;
                control_state_t ctrl; app_shared_get_control(&ctrl);
                float sp = ctrl.setpoint_c + APP_SETPOINT_STEP;
                if (sp > APP_SETPOINT_MAX) sp = APP_SETPOINT_MAX;
                app_shared_set_setpoint(sp);
            } else if (ch == '-') {
                s_idx = 0U; s_target = INPUT_SP;
                control_state_t ctrl; app_shared_get_control(&ctrl);
                float sp = ctrl.setpoint_c - APP_SETPOINT_STEP;
                if (sp < APP_SETPOINT_MIN) sp = APP_SETPOINT_MIN;
                app_shared_set_setpoint(sp);
            } else if (ch == 'p' || ch == 'P') {
                s_idx = 0U; s_target = INPUT_KP;
                printf("[IO] Enter Kp: ");
            } else if (ch == 'i' || ch == 'I') {
                s_idx = 0U; s_target = INPUT_KI;
                printf("[IO] Enter Ki: ");
            } else if (ch == 'k' || ch == 'K') {
                s_idx = 0U; s_target = INPUT_KD;
                printf("[IO] Enter Kd: ");
            } else if (ch == 'D' || ch == 'd') {
                s_idx = 0U;
                app_shared_toggle_display();
            } else if (ch == '?') {
                s_idx = 0U;
                control_state_t ctrl; sensor_data_t sensor;
                relay_state_t relay; pid_params_t params;
                app_shared_get_control(&ctrl); app_shared_get_sensor(&sensor);
                app_shared_get_relay(&relay);  app_shared_get_pid_params(&params);
                char sp[8], t[8], pid_o[8], kp[8], ki[8], kd[8];
                dtostrf(ctrl.setpoint_c,      5, 1, sp);
                dtostrf(sensor.temperature_c, 5, 1, t);
                dtostrf(ctrl.pid_output,      4, 2, pid_o);
                dtostrf(params.kp, 5, 2, kp);
                dtostrf(params.ki, 5, 3, ki);
                dtostrf(params.kd, 5, 3, kd);
                printf("SP=%sC T=%sC PID=%s FAN=%s Kp=%s Ki=%s Kd=%s\n",
                       sp, t, pid_o,
                       relay.is_on ? "ON" : "OFF",
                       kp, ki, kd);
            } else if (ch == '/') {
                s_idx = 0U;
                printf("--- Serial commands ---\n");
                printf("  +/-        : raise/lower setpoint by 0.5C\n");
                printf("  XX.X<Enter>: set setpoint directly (e.g. 26.5)\n");
                printf("  p<Enter>   : enter new Kp gain\n");
                printf("  i<Enter>   : enter new Ki gain\n");
                printf("  k<Enter>   : enter new Kd gain\n");
                printf("  D          : toggle LCD on/off\n");
                printf("  ?          : print current status\n");
                printf("  /          : print this help\n");
            } else if (ch == '\r' || ch == '\n') {
                if (s_idx > 0U) {
                    s_buf[s_idx] = '\0'; s_idx = 0U;
                    float val = (float)atof(s_buf);
                    if (s_target == INPUT_SP) {
                        if (val >= APP_SETPOINT_MIN && val <= APP_SETPOINT_MAX)
                            app_shared_set_setpoint(val);
                    } else if (val >= 0.0f) {
                        pid_params_t p2; app_shared_get_pid_params(&p2);
                        if      (s_target == INPUT_KP) { p2.kp = val; pid.kp = val; }
                        else if (s_target == INPUT_KI) { p2.ki = val; pid.ki = val; }
                        else                           { p2.kd = val; pid.kd = val; }
                        app_shared_set_pid_params(&p2);
                    }
                }
                s_target = INPUT_SP;
            } else if ((ch >= '0' && ch <= '9') || ch == '.') {
                if (s_idx < (uint8_t)(sizeof(s_buf) - 1U))
                    s_buf[s_idx++] = (char)ch;
            } else {
                s_idx = 0U; s_target = INPUT_SP;
            }
        }

        /* sync gains from shared state */
        pid_params_t params; app_shared_get_pid_params(&params);
        pid.kp = params.kp; pid.ki = params.ki; pid.kd = params.kd;

        /* PID update */
        sensor_data_t sensor; control_state_t ctrl;
        app_shared_get_sensor(&sensor);
        app_shared_get_control(&ctrl);

        if (sensor.valid) {
            float out = pid_update(&pid, ctrl.setpoint_c, sensor.temperature_c);
            app_shared_set_pid_output(out, now);
        } else {
            pid_reset(&pid);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LAB5_2_TASK_CTRL_MS));
    }
}
