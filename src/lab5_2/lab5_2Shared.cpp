#include "lab5_2Shared.h"

void lab5_2SharedInit(void)
{
    app_shared_init();
    app_shared_set_algo(CTRL_ALGO_PID);
}
