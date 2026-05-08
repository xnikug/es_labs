#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float kp, ki, kd;
    float dt;
    float integral;
    float prevError;
    float outputMin;
    float outputMax;
} PidState_t;

void  pidInit(PidState_t *pid,
              float kp, float ki, float kd,
              float dt,
              float outMin, float outMax);

float pidUpdate(PidState_t *pid, float setpoint, float measurement);
void  pidReset(PidState_t *pid);

#ifdef __cplusplus
}
#endif
