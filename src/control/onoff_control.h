#pragma once

#include "../app/app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

relay_cmd_t onoff_control_update(float measurement,
                                 float setpoint,
                                 float hysteresis,
                                 relay_cmd_t current_output);

#ifdef __cplusplus
}
#endif
