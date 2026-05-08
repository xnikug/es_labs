#include "lab5_2Shared.h"

void lab5_2SharedInit(void)
{
    appSharedInit();
    appSharedSetAlgo(CTRL_ALGO_PID);
}
