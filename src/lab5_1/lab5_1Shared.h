#ifndef LAB5_1_SHARED_H
#define LAB5_1_SHARED_H

#include "app_config.h"
#include "../app_shared/app_shared.h"

#define LAB5_1_TASK_ACQ_MS   APP_TASK_SENSOR_MS
#define LAB5_1_TASK_COND_MS  APP_TASK_IO_MS
#define LAB5_1_TASK_CTRL_MS  APP_TASK_CONTROL_MS
#define LAB5_1_TASK_ACT_MS   APP_TASK_ACTUATOR_MS
#define LAB5_1_TASK_REP_MS   APP_TASK_DISPLAY_MS

void lab5_1SharedInit(void);

#endif
