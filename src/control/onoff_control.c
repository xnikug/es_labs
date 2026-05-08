#include "onoff_control.h"

relay_cmd_t onoff_control_update(float measurement,
                                 float setpoint,
                                 float hysteresis,
                                 relay_cmd_t current_output)
{
    if (measurement > setpoint + hysteresis) return RELAY_CMD_ON;
    if (measurement < setpoint - hysteresis) return RELAY_CMD_OFF;
    return current_output;
}
