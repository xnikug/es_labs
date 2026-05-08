#ifndef LAB5_2_PID_CONTROLLER_H
#define LAB5_2_PID_CONTROLLER_H

void  lab5_2PidInit(float kp, float ki, float kd);
float lab5_2PidUpdate(float measured, float setPoint, float dtSeconds);
void  lab5_2PidReset(void);
void  lab5_2PidSetGains(float kp, float ki, float kd);
float lab5_2PidGetKp(void);
float lab5_2PidGetKi(void);
float lab5_2PidGetKd(void);

#endif
