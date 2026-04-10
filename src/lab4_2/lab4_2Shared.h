#ifndef LAB4_2_SHARED_H
#define LAB4_2_SHARED_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "app_config.h"

enum lab4_2MotorHealth_t
{
    LAB4_2_MOTOR_IDLE = 0,
    LAB4_2_MOTOR_ACTIVE,
    LAB4_2_MOTOR_HOT
};

static const int LAB4_2_TASK_OFFSET_MS = 1000;
static const int LAB4_2_CONTROL_PERIOD_MS = TASK_COND_MS;
static const int LAB4_2_REPORT_PERIOD_MS = TASK_DISP_MS;
static const int LAB4_2_INPUT_SCAN_PERIOD_MS = TASK_CMD_MS;
static const int LAB4_2_ALERT_PERIOD_MS = TASK_ALERT_MS;

static const int LAB4_2_POWER_MIN = MOTOR_MIN_SPD;
static const int LAB4_2_POWER_MAX = MOTOR_MAX_SPD;

static const int LAB4_2_RAMP_DELTA_PER_CYCLE = MOTOR_RAMP_STEP;
static const int LAB4_2_KICKSTART_POWER = 70;
static const int LAB4_2_KICKSTART_TIME_MS = 150;

static const int LAB4_2_HOT_THRESHOLD = MOTOR_ALERT_HIGH;
static const int LAB4_2_HOT_COUNTER_LIMIT = 10;

extern int gLab4_2TargetPower;
extern int gLab4_2AppliedPower;
extern lab4_2MotorHealth_t gLab4_2Health;
extern int gLab4_2HotCounter;

extern uint8_t gLab4_2InputSource;
extern uint8_t gLab4_2OutputMode;
extern int gLab4_2SerialControlPower;
extern SemaphoreHandle_t gLab4_2PowerMutex;

extern int gLab4_2SerialRaw;
extern int gLab4_2SerialSat;
extern int gLab4_2SerialMedian;
extern int gLab4_2SerialWeighted;

extern int gLab4_2LastSerialValue;

void lab4_2SharedInit();

int lab4_2ClampInt(int value, int minValue, int maxValue);
int lab4_2StepTowards(int current, int target, int maxStep);
int lab4_2MedianFilter5(int sample);
int lab4_2WeightedFilter5(int sample);

void lab4_2MotorOutInit();
void lab4_2MotorOutSetPercent(int signedPercent);
void lab4_2RelayInit();
void lab4_2RelaySet(bool on);
void lab4_2AlertLedInit();
void lab4_2AlertLedSet(bool on);

const char *lab4_2HealthToText(lab4_2MotorHealth_t health);

#endif
