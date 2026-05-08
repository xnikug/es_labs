#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void    dht22_stdio_init(uint8_t pin);
uint8_t dht22_read(float *temperature_c, float *humidity_pct);

#ifdef __cplusplus
}
#endif
