#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void  uart_stdio_init(uint32_t baud);
FILE* uart_stdio_stream(void);
int   uart_getchar_nb(void);

#ifdef __cplusplus
}
#endif
