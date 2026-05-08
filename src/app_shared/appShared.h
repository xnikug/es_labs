#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../app/appTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void appSharedInit(void);

void appSharedSetSensor(const SensorData_t *data);
void appSharedGetSensor(SensorData_t *data);

void appSharedSetSetpoint(float sp);
void appSharedSetHysteresis(float hyst);
void appSharedSetAlgo(CtrlAlgo_t algo);
void appSharedSetOnoffOutput(RelayCmd_t cmd, uint32_t ts);
void appSharedSetPidOutput(float output, uint32_t ts);
void appSharedGetControl(ControlState_t *out);

void appSharedSetPidParams(const PidParams_t *p);
void appSharedGetPidParams(PidParams_t *p);

void appSharedSetRelay(const RelayState_t *state);
void appSharedGetRelay(RelayState_t *state);

void appSharedToggleDisplay(void);
void appSharedGetUi(UiState_t *ui);

#ifdef __cplusplus
}
#endif
