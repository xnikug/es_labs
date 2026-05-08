#include "pid_control.h"

void pid_init(pid_state_t *pid,
              float kp, float ki, float kd,
              float dt,
              float out_min, float out_max)
{
    pid->kp         = kp;
    pid->ki         = ki;
    pid->kd         = kd;
    pid->dt         = dt;
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
}

float pid_update(pid_state_t *pid, float setpoint, float measurement)
{
    float error      = measurement - setpoint;
    float derivative = (error - pid->prev_error) / pid->dt;

    pid->integral += error * pid->dt;
    if (pid->ki > 1e-6f)
    {
        float ilim = (pid->output_max - pid->output_min) / pid->ki;
        if (pid->integral >  ilim) pid->integral =  ilim;
        if (pid->integral < -ilim) pid->integral = -ilim;
    }

    pid->prev_error = error;

    float output = pid->kp * error
                 + pid->ki * pid->integral
                 + pid->kd * derivative;

    if (output > pid->output_max) output = pid->output_max;
    if (output < pid->output_min) output = pid->output_min;
    return output;
}

void pid_reset(pid_state_t *pid)
{
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
}
