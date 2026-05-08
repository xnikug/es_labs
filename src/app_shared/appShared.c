#include "appShared.h"
#include "../app_config.h"
#include <string.h>

#if defined(__AVR_ATmega2560__)
  #include <Arduino_FreeRTOS.h>
  #include <semphr.h>
  #define MTX_T        SemaphoreHandle_t
  #define MTX_CREATE() xSemaphoreCreateMutex()
  #define MTX_TAKE(m)  xSemaphoreTake((m), portMAX_DELAY)
  #define MTX_GIVE(m)  xSemaphoreGive(m)
#else
  #define MTX_T        uint8_t
  #define MTX_CREATE() 0U
  #define MTX_TAKE(m)  ((void)(m))
  #define MTX_GIVE(m)  ((void)(m))
#endif

static SensorData_t   g_sensor;
static MTX_T          g_mtxSensor;

static ControlState_t g_control;
static MTX_T          g_mtxControl;

static PidParams_t    g_pidParams;
static MTX_T          g_mtxPidParams;

static RelayState_t   g_relay;
static MTX_T          g_mtxRelay;

static UiState_t      g_ui;
static MTX_T          g_mtxUi;

void appSharedInit(void)
{
    g_mtxSensor    = MTX_CREATE();
    g_mtxControl   = MTX_CREATE();
    g_mtxPidParams = MTX_CREATE();
    g_mtxRelay     = MTX_CREATE();
    g_mtxUi        = MTX_CREATE();

    memset(&g_sensor, 0, sizeof(g_sensor));
    memset(&g_relay,  0, sizeof(g_relay));
    memset(&g_ui,     0, sizeof(g_ui));

    g_control.setpointC   = APP_SETPOINT_DEFAULT;
    g_control.hysteresisC = APP_HYSTERESIS_DEFAULT;
    g_control.algo        = CTRL_ALGO_ONOFF;
    g_control.onoffOutput = RELAY_CMD_OFF;
    g_control.pidOutput   = 0.0f;
    g_control.timestampMs = 0UL;

    g_pidParams.kp = APP_PID_KP_DEFAULT;
    g_pidParams.ki = APP_PID_KI_DEFAULT;
    g_pidParams.kd = APP_PID_KD_DEFAULT;

    g_ui.displayEnabled = 1U;
    g_ui.page           = 0U;
    g_ui.lastUpdateMs   = 0UL;
}

void appSharedSetSensor(const SensorData_t *data)
{ MTX_TAKE(g_mtxSensor); g_sensor = *data; MTX_GIVE(g_mtxSensor); }
void appSharedGetSensor(SensorData_t *data)
{ MTX_TAKE(g_mtxSensor); *data = g_sensor; MTX_GIVE(g_mtxSensor); }

void appSharedSetSetpoint(float sp)
{ MTX_TAKE(g_mtxControl); g_control.setpointC = sp; MTX_GIVE(g_mtxControl); }
void appSharedSetHysteresis(float hyst)
{ MTX_TAKE(g_mtxControl); g_control.hysteresisC = hyst; MTX_GIVE(g_mtxControl); }
void appSharedSetAlgo(CtrlAlgo_t algo)
{ MTX_TAKE(g_mtxControl); g_control.algo = algo; MTX_GIVE(g_mtxControl); }
void appSharedSetOnoffOutput(RelayCmd_t cmd, uint32_t ts)
{ MTX_TAKE(g_mtxControl); g_control.onoffOutput = cmd; g_control.timestampMs = ts; MTX_GIVE(g_mtxControl); }
void appSharedSetPidOutput(float output, uint32_t ts)
{ MTX_TAKE(g_mtxControl); g_control.pidOutput = output; g_control.timestampMs = ts; MTX_GIVE(g_mtxControl); }
void appSharedGetControl(ControlState_t *out)
{ MTX_TAKE(g_mtxControl); *out = g_control; MTX_GIVE(g_mtxControl); }

void appSharedSetPidParams(const PidParams_t *p)
{ MTX_TAKE(g_mtxPidParams); g_pidParams = *p; MTX_GIVE(g_mtxPidParams); }
void appSharedGetPidParams(PidParams_t *p)
{ MTX_TAKE(g_mtxPidParams); *p = g_pidParams; MTX_GIVE(g_mtxPidParams); }

void appSharedSetRelay(const RelayState_t *state)
{ MTX_TAKE(g_mtxRelay); g_relay = *state; MTX_GIVE(g_mtxRelay); }
void appSharedGetRelay(RelayState_t *state)
{ MTX_TAKE(g_mtxRelay); *state = g_relay; MTX_GIVE(g_mtxRelay); }

void appSharedToggleDisplay(void)
{
    MTX_TAKE(g_mtxUi);
    g_ui.displayEnabled = (g_ui.displayEnabled == 0U) ? 1U : 0U;
    MTX_GIVE(g_mtxUi);
}

void appSharedGetUi(UiState_t *ui)
{ MTX_TAKE(g_mtxUi); *ui = g_ui; MTX_GIVE(g_mtxUi); }
