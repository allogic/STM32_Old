#ifndef TERMINAL_H
#define TERMINAL_H

#define TERMINAL_PORT_TX GPIOA
#define TERMINAL_PIN_TX GPIO9
#define TERMINAL_AF_TX GPIO_AF7

#define TERMINAL_PORT_RX GPIOA
#define TERMINAL_PIN_RX GPIO10
#define TERMINAL_AF_RX GPIO_AF7

#define PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T

#include <printf/printf.h>

void terminal_init(void);

#endif