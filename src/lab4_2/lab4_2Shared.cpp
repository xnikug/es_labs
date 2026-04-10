#include "lab4_2Shared.h"

int gLab4_2TargetPower = 0;
int gLab4_2AppliedPower = 0;
lab4_2MotorHealth_t gLab4_2Health = LAB4_2_MOTOR_IDLE;
int gLab4_2HotCounter = 0;

uint8_t gLab4_2InputSource = 0;
uint8_t gLab4_2OutputMode = 0;
int gLab4_2SerialControlPower = 0;
SemaphoreHandle_t gLab4_2PowerMutex = NULL;

int gLab4_2SerialRaw = 0;
int gLab4_2SerialSat = 0;
int gLab4_2SerialMedian = 0;
int gLab4_2SerialWeighted = 0;

int gLab4_2LastSerialValue = 0;

static int gLab4_2MedianFifo[5] = {0, 0, 0, 0, 0};
static int gLab4_2WeightedFifo[5] = {0, 0, 0, 0, 0};
static const int gLab4_2Weights[5] = {50, 20, 15, 10, 5};

static void shiftRightAndInsert(int *buffer, int size, int sample)
{
    for (int i = size - 1; i > 0; --i) {
        buffer[i] = buffer[i - 1];
    }
    buffer[0] = sample;
}

void lab4_2SharedInit()
{
    gLab4_2TargetPower = 0;
    gLab4_2AppliedPower = 0;
    gLab4_2Health = LAB4_2_MOTOR_IDLE;
    gLab4_2HotCounter = 0;

    gLab4_2SerialControlPower = 0;
    gLab4_2SerialRaw = 0;
    gLab4_2SerialSat = 0;
    gLab4_2SerialMedian = 0;
    gLab4_2SerialWeighted = 0;
    gLab4_2LastSerialValue = 0;
    gLab4_2InputSource = 1;

    for (int i = 0; i < 5; ++i) {
        gLab4_2MedianFifo[i] = 0;
        gLab4_2WeightedFifo[i] = 0;
    }
}

int lab4_2ClampInt(int value, int minValue, int maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

int lab4_2StepTowards(int current, int target, int maxStep)
{
    const int delta = target - current;
    if (delta > maxStep) {
        return current + maxStep;
    }
    if (delta < -maxStep) {
        return current - maxStep;
    }
    return target;
}

int lab4_2MedianFilter5(int sample)
{
    shiftRightAndInsert(gLab4_2MedianFifo, 5, sample);

    int sorted[5];
    for (int i = 0; i < 5; ++i) {
        sorted[i] = gLab4_2MedianFifo[i];
    }

    for (int i = 1; i < 5; ++i) {
        int key = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > key) {
            sorted[j + 1] = sorted[j];
            --j;
        }
        sorted[j + 1] = key;
    }

    return sorted[2];
}

int lab4_2WeightedFilter5(int sample)
{
    shiftRightAndInsert(gLab4_2WeightedFifo, 5, sample);

    int acc = 0;
    int weightSum = 0;
    for (int i = 0; i < 5; ++i) {
        acc += gLab4_2WeightedFifo[i] * gLab4_2Weights[i];
        weightSum += gLab4_2Weights[i];
    }

    return (weightSum == 0) ? sample : (acc / weightSum);
}

void lab4_2MotorOutInit()
{
    pinMode(MOTOR_ENA_PIN, OUTPUT);
    pinMode(MOTOR_IN1_PIN, OUTPUT);
    pinMode(MOTOR_IN2_PIN, OUTPUT);

    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    analogWrite(MOTOR_ENA_PIN, 0);
}

void lab4_2MotorOutSetPercent(int signedPercent)
{
    const int clipped = lab4_2ClampInt(signedPercent, LAB4_2_POWER_MIN, LAB4_2_POWER_MAX);
    const int magnitude = (clipped < 0) ? -clipped : clipped;
    const int pwm = map(magnitude, 0, 100, 0, 255);

    if (clipped > 0) {
        digitalWrite(MOTOR_IN1_PIN, HIGH);
        digitalWrite(MOTOR_IN2_PIN, LOW);
    } else if (clipped < 0) {
        digitalWrite(MOTOR_IN1_PIN, LOW);
        digitalWrite(MOTOR_IN2_PIN, HIGH);
    } else {
        digitalWrite(MOTOR_IN1_PIN, LOW);
        digitalWrite(MOTOR_IN2_PIN, LOW);
    }

    analogWrite(MOTOR_ENA_PIN, pwm);
}

void lab4_2RelayInit()
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
}

void lab4_2RelaySet(bool on)
{
    digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}

void lab4_2AlertLedInit()
{
    pinMode(ALERT_LED_PIN, OUTPUT);
    digitalWrite(ALERT_LED_PIN, LOW);
}

void lab4_2AlertLedSet(bool on)
{
    digitalWrite(ALERT_LED_PIN, on ? HIGH : LOW);
}

const char *lab4_2HealthToText(lab4_2MotorHealth_t health)
{
    switch (health) {
    case LAB4_2_MOTOR_IDLE:
        return "IDLE";
    case LAB4_2_MOTOR_ACTIVE:
        return "RUN ";
    case LAB4_2_MOTOR_HOT:
        return "HOT!";
    default:
        return "???";
    }
}
