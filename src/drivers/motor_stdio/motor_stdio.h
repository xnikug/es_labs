#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void motor_stdio_init(uint8_t pin);
void motor_set_state(uint8_t on);

#ifdef __cplusplus
}
#endif
