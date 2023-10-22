#ifndef DBG_H
#define DBG_H

#include "stm32f1xx_hal_uart.h"

UART_HandleTypeDef *get_uart3(void);

#define DBG(fmt, ...) { \
    char dbg_buf[256] = "\0"; \
    sprintf(dbg_buf, fmt, __VA_ARGS__); \
    HAL_UART_Transmit(get_uart3(), dbg_buf, strlen(dbg_buf), 100); \
}

#endif
