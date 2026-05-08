#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../app/app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void app_shared_init(void);

void app_shared_set_sensor(const sensor_data_t *data);
void app_shared_get_sensor(sensor_data_t *data);

void app_shared_set_setpoint(float sp);
void app_shared_set_hysteresis(float hyst);
void app_shared_set_algo(ctrl_algo_t algo);
void app_shared_set_onoff_output(relay_cmd_t cmd, uint32_t ts);
void app_shared_set_pid_output(float output, uint32_t ts);
void app_shared_get_control(control_state_t *out);

void app_shared_set_pid_params(const pid_params_t *p);
void app_shared_get_pid_params(pid_params_t *p);

void app_shared_set_relay(const relay_state_t *state);
void app_shared_get_relay(relay_state_t *state);

void app_shared_toggle_display(void);
void app_shared_get_ui(ui_state_t *ui);

#ifdef __cplusplus
}
#endif
