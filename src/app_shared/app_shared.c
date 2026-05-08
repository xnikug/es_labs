#include "app_shared.h"
#include "../app_config.h"
#include <string.h>

#if defined(__AVR_ATmega2560__)
  #include <Arduino_FreeRTOS.h>
  #include <semphr.h>
  #define MTX_T        SemaphoreHandle_t
  #define MTX_CREATE() xSemaphoreCreateMutex()
  #define MTX_TAKE(m)  xSemaphoreTake((m), portMAX_DELAY)
  #define MTX_GIVE(m)   xSemaphoreGive(m)
#else
  #define MTX_T        uint8_t
  #define MTX_CREATE()  0U
  #define MTX_TAKE(m)   ((void)(m))
  #define MTX_GIVE(m)   ((void)(m))
#endif

static sensor_data_t   g_sensor;
static MTX_T           g_mtx_sensor;

static control_state_t g_control;
static MTX_T           g_mtx_control;

static pid_params_t    g_pid_params;
static MTX_T           g_mtx_pid_params;

static relay_state_t   g_relay;
static MTX_T           g_mtx_relay;

static ui_state_t      g_ui;
static MTX_T           g_mtx_ui;

void app_shared_init(void)
{
    g_mtx_sensor     = MTX_CREATE();
    g_mtx_control    = MTX_CREATE();
    g_mtx_pid_params = MTX_CREATE();
    g_mtx_relay      = MTX_CREATE();
    g_mtx_ui         = MTX_CREATE();

    memset(&g_sensor, 0, sizeof(g_sensor));
    memset(&g_relay,  0, sizeof(g_relay));
    memset(&g_ui,     0, sizeof(g_ui));

    g_control.setpoint_c   = APP_SETPOINT_DEFAULT;
    g_control.hysteresis_c = APP_HYSTERESIS_DEFAULT;
    g_control.algo         = CTRL_ALGO_ONOFF;
    g_control.onoff_output = RELAY_CMD_OFF;
    g_control.pid_output   = 0.0f;
    g_control.timestamp_ms = 0UL;

    g_pid_params.kp = APP_PID_KP_DEFAULT;
    g_pid_params.ki = APP_PID_KI_DEFAULT;
    g_pid_params.kd = APP_PID_KD_DEFAULT;

    g_ui.display_enabled = 1U;
    g_ui.page = 0U;
    g_ui.last_update_ms = 0UL;
}

void app_shared_set_sensor(const sensor_data_t *data)
{ MTX_TAKE(g_mtx_sensor); g_sensor = *data; MTX_GIVE(g_mtx_sensor); }
void app_shared_get_sensor(sensor_data_t *data)
{ MTX_TAKE(g_mtx_sensor); *data = g_sensor; MTX_GIVE(g_mtx_sensor); }

void app_shared_set_setpoint(float sp)
{ MTX_TAKE(g_mtx_control); g_control.setpoint_c = sp; MTX_GIVE(g_mtx_control); }
void app_shared_set_hysteresis(float hyst)
{ MTX_TAKE(g_mtx_control); g_control.hysteresis_c = hyst; MTX_GIVE(g_mtx_control); }
void app_shared_set_algo(ctrl_algo_t algo)
{ MTX_TAKE(g_mtx_control); g_control.algo = algo; MTX_GIVE(g_mtx_control); }
void app_shared_set_onoff_output(relay_cmd_t cmd, uint32_t ts)
{ MTX_TAKE(g_mtx_control); g_control.onoff_output = cmd; g_control.timestamp_ms = ts; MTX_GIVE(g_mtx_control); }
void app_shared_set_pid_output(float output, uint32_t ts)
{ MTX_TAKE(g_mtx_control); g_control.pid_output = output; g_control.timestamp_ms = ts; MTX_GIVE(g_mtx_control); }
void app_shared_get_control(control_state_t *out)
{ MTX_TAKE(g_mtx_control); *out = g_control; MTX_GIVE(g_mtx_control); }

void app_shared_set_pid_params(const pid_params_t *p)
{ MTX_TAKE(g_mtx_pid_params); g_pid_params = *p; MTX_GIVE(g_mtx_pid_params); }
void app_shared_get_pid_params(pid_params_t *p)
{ MTX_TAKE(g_mtx_pid_params); *p = g_pid_params; MTX_GIVE(g_mtx_pid_params); }

void app_shared_set_relay(const relay_state_t *state)
{ MTX_TAKE(g_mtx_relay); g_relay = *state; MTX_GIVE(g_mtx_relay); }
void app_shared_get_relay(relay_state_t *state)
{ MTX_TAKE(g_mtx_relay); *state = g_relay; MTX_GIVE(g_mtx_relay); }

void app_shared_toggle_display(void)
{
    MTX_TAKE(g_mtx_ui);
    g_ui.display_enabled = (g_ui.display_enabled == 0U) ? 1U : 0U;
    MTX_GIVE(g_mtx_ui);
}

void app_shared_get_ui(ui_state_t *ui)
{ MTX_TAKE(g_mtx_ui); *ui = g_ui; MTX_GIVE(g_mtx_ui); }
