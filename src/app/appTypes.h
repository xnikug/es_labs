#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    float    temperatureC;
    float    humidityPct;
    uint8_t  valid;
    uint32_t timestampMs;
} SensorData_t;

typedef enum
{
    CTRL_ALGO_ONOFF = 0,
    CTRL_ALGO_PID   = 1,
} CtrlAlgo_t;

typedef enum
{
    RELAY_CMD_OFF = 0,
    RELAY_CMD_ON  = 1,
} RelayCmd_t;

typedef struct
{
    float kp;
    float ki;
    float kd;
} PidParams_t;

typedef struct
{
    float       setpointC;
    float       hysteresisC;
    CtrlAlgo_t  algo;
    RelayCmd_t  onoffOutput;
    float       pidOutput;
    uint32_t    timestampMs;
} ControlState_t;

typedef struct
{
    uint8_t  isOn;
    uint32_t onCycles;
    uint32_t offCycles;
    uint32_t stableMs;
} RelayState_t;

typedef struct
{
    uint8_t  displayEnabled;
    uint8_t  page;
    uint32_t lastUpdateMs;
} UiState_t;
