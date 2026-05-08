#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float kp, ki, kd;
    float dt;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
} pid_state_t;

void  pid_init(pid_state_t *pid,
               float kp, float ki, float kd,
               float dt,
               float out_min, float out_max);

float pid_update(pid_state_t *pid, float setpoint, float measurement);
void  pid_reset(pid_state_t *pid);

#ifdef __cplusplus
}
#endif
