#pragma once

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void  lcd_stdio_init(uint8_t addr, uint8_t cols, uint8_t rows,
                     uint8_t sda_pin, uint8_t scl_pin);
FILE* lcd_stdio_stream(void);

#ifdef __cplusplus
}
#endif
