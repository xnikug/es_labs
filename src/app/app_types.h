#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    float    temperature_c;
    float    humidity_pct;
    uint8_t  valid;
    uint32_t timestamp_ms;
} sensor_data_t;

typedef enum
{
    CTRL_ALGO_ONOFF = 0,
    CTRL_ALGO_PID   = 1,
} ctrl_algo_t;

typedef enum
{
    RELAY_CMD_OFF = 0,
    RELAY_CMD_ON  = 1,
} relay_cmd_t;

typedef struct
{
    float kp;
    float ki;
    float kd;
} pid_params_t;

typedef struct
{
    float       setpoint_c;
    float       hysteresis_c;
    ctrl_algo_t algo;
    relay_cmd_t onoff_output;
    float       pid_output;   /* 0..1 relay ON fraction per cycle */
    uint32_t    timestamp_ms;
} control_state_t;

typedef struct
{
    uint8_t  is_on;
    uint32_t on_cycles;
    uint32_t off_cycles;
    uint32_t stable_ms;
} relay_state_t;

typedef struct
{
    uint8_t  display_enabled;
    uint8_t  page;
    uint32_t last_update_ms;
} ui_state_t;
