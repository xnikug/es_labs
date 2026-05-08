#pragma once

#include "../app/appTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

RelayCmd_t onoffControlUpdate(float measurement,
                              float setpoint,
                              float hysteresis,
                              RelayCmd_t currentOutput);

#ifdef __cplusplus
}
#endif
