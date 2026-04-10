#pragma once

// Hardware pins
#define RELAY_PIN 7

#define MOTOR_ENA_PIN 2
#define MOTOR_IN1_PIN 3
#define MOTOR_IN2_PIN 4

#define ALERT_LED_PIN 13

// Relay parameters
#define RELAY_DEBOUNCE_CNT 3

// Motor parameters
#define MOTOR_MIN_SPD (-100)
#define MOTOR_MAX_SPD 100
#define MOTOR_STOP 0

#define MOTOR_ALERT_LOW (-85)
#define MOTOR_ALERT_HIGH 85

#define MOTOR_RAMP_STEP 5
#define MOTOR_WMA_ALPHA 0.3f
#define MEDIAN_WINDOW 5

// Task periods [ms]
#define TASK_CMD_MS 50
#define TASK_COND_MS 50
#define TASK_ALERT_MS 100
#define TASK_DISP_MS 500
