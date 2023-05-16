#ifndef TERMINAL_H
#define TERMINAL_H

#define PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T

#include <printf/printf.h>

void terminal_init(void);

#endif