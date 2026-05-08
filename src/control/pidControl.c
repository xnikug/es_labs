#include "pidControl.h"

void pidInit(PidState_t *pid,
             float kp, float ki, float kd,
             float dt,
             float outMin, float outMax)
{
    pid->kp        = kp;
    pid->ki        = ki;
    pid->kd        = kd;
    pid->dt        = dt;
    pid->outputMin = outMin;
    pid->outputMax = outMax;
    pid->integral  = 0.0f;
    pid->prevError = 0.0f;
}

float pidUpdate(PidState_t *pid, float setpoint, float measurement)
{
    float error      = measurement - setpoint;
    float derivative = (error - pid->prevError) / pid->dt;

    pid->integral += error * pid->dt;
    if (pid->ki > 1e-6f)
    {
        float ilim = (pid->outputMax - pid->outputMin) / pid->ki;
        if (pid->integral >  ilim) pid->integral =  ilim;
        if (pid->integral < -ilim) pid->integral = -ilim;
    }

    pid->prevError = error;

    float output = pid->kp * error
                 + pid->ki * pid->integral
                 + pid->kd * derivative;

    if (output > pid->outputMax) output = pid->outputMax;
    if (output < pid->outputMin) output = pid->outputMin;
    return output;
}

void pidReset(PidState_t *pid)
{
    pid->integral  = 0.0f;
    pid->prevError = 0.0f;
}
