#ifndef LAB5_2_SHARED_H
#define LAB5_2_SHARED_H

#include "app_config.h"
#include "../app_shared/appShared.h"

#define LAB5_2_TASK_ACQ_MS     APP_TASK_SENSOR_MS
#define LAB5_2_TASK_CTRL_MS    100U
#define LAB5_2_TASK_OUTPUT_MS  APP_TASK_ACTUATOR_MS
#define LAB5_2_TASK_REPORT_MS  APP_TASK_DISPLAY_MS

void lab5_2SharedInit(void);

#endif
