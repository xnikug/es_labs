#include "ddSnsAngle.h"

static int gAngleValueDeg = 0;
static SemaphoreHandle_t gAngleMutex = NULL;

void ddSnsAngleSetup()
{
    gAngleMutex = xSemaphoreCreateMutex();
}

SemaphoreHandle_t ddSnsAngleGetMutex()
{
    return gAngleMutex;
}

int ddSnsAngleGetValue()
{
    int currentValue = gAngleValueDeg;
    if (gAngleMutex != NULL) {
        xSemaphoreTake(gAngleMutex, portMAX_DELAY);
        currentValue = gAngleValueDeg;
        xSemaphoreGive(gAngleMutex);
    }
    return currentValue;
}

void ddSnsAngleSetValue(int angleDeg)
{
    if (gAngleMutex != NULL) {
        xSemaphoreTake(gAngleMutex, portMAX_DELAY);
        gAngleValueDeg = angleDeg;
        xSemaphoreGive(gAngleMutex);
        return;
    }

    gAngleValueDeg = angleDeg;
}
