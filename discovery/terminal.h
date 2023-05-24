#ifndef TERMINAL_H
#define TERMINAL_H

#define TERMINAL_PORT_TX GPIOA
#define TERMINAL_PIN_TX GPIO2
#define TERMINAL_AF_TX GPIO_AF7

#define TERMINAL_PORT_RX GPIOA
#define TERMINAL_PIN_RX GPIO3
#define TERMINAL_AF_RX GPIO_AF7

#define TERMINAL_CHAR_QUEUE_SIZE 512

#define PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T

#include <stdbool.h>

#include <printf/printf.h>

void terminal_init(void);
bool terminal_arg(const char* value);

#endif