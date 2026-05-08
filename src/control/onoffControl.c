#include "onoffControl.h"

RelayCmd_t onoffControlUpdate(float measurement,
                              float setpoint,
                              float hysteresis,
                              RelayCmd_t currentOutput)
{
    if (measurement > setpoint + hysteresis) return RELAY_CMD_ON;
    if (measurement < setpoint - hysteresis) return RELAY_CMD_OFF;
    return currentOutput;
}
