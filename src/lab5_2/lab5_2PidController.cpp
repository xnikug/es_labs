#include "lab5_2PidController.h"
#include "app_config.h"

static float gKp       = APP_PID_KP_DEFAULT;
static float gKi       = APP_PID_KI_DEFAULT;
static float gKd       = APP_PID_KD_DEFAULT;
static float gIntegral = 0.0f;
static float gPrevErr  = 0.0f;

void lab5_2PidInit(float kp, float ki, float kd)
{
    gKp = kp; gKi = ki; gKd = kd;
    gIntegral = 0.0f; gPrevErr = 0.0f;
}

float lab5_2PidUpdate(float measured, float setPoint, float dtSeconds)
{
    if (dtSeconds <= 0.0f) dtSeconds = 0.001f;
    float error = setPoint - measured;
    gIntegral += error * dtSeconds;
    if (gIntegral >  50.0f) gIntegral =  50.0f;
    if (gIntegral < -50.0f) gIntegral = -50.0f;
    float dTerm = gKd * (error - gPrevErr) / dtSeconds;
    gPrevErr = error;
    float out = gKp * error + gKi * gIntegral + dTerm;
    if (out > 100.0f) out = 100.0f;
    if (out <   0.0f) out =   0.0f;
    return out;
}

void  lab5_2PidReset(void)       { gIntegral = 0.0f; gPrevErr = 0.0f; }
void  lab5_2PidSetGains(float kp, float ki, float kd) { gKp=kp; gKi=ki; gKd=kd; }
float lab5_2PidGetKp(void)       { return gKp; }
float lab5_2PidGetKi(void)       { return gKi; }
float lab5_2PidGetKd(void)       { return gKd; }
