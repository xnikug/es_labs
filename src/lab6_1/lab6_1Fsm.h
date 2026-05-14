#pragma once
#include <stdint.h>

void     lab6_1FsmInit(void);
uint8_t  lab6_1FsmReadInput(void);    /* snapshot: 1 = button held LOW      */
uint8_t  lab6_1FsmPollButton(void);   /* edge: 1 once per validated press    */
void     lab6_1FsmApplyOutput(void);  /* LED = f(currentState)               */
void     lab6_1FsmStep(uint8_t input);
uint8_t  lab6_1FsmGetState(void);
uint16_t lab6_1FsmGetDelay(void);
