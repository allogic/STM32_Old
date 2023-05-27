#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

#define TERMINAL_PORT_TX GPIOA
#define TERMINAL_PIN_TX GPIO2
#define TERMINAL_AF_TX GPIO_AF7

#define TERMINAL_PORT_RX GPIOA
#define TERMINAL_PIN_RX GPIO3
#define TERMINAL_AF_RX GPIO_AF7

#define TERMINAL_ESC "\033"
#define TERMINAL_CSI TERMINAL_ESC "["

#define TERMINAL_CSI_CUB(N) TERMINAL_CSI #N "D"
#define TERMINAL_CSI_CUF(N) TERMINAL_CSI #N "C"
#define TERMINAL_CSI_CCU(N) TERMINAL_CSI #N "A"
#define TERMINAL_CSI_CUD(N) TERMINAL_CSI #N "B"

#define TERMINAL_CSI_INSERT TERMINAL_CSI "2~"
#define TERMINAL_CSI_DELETE TERMINAL_CSI "3~"
#define TERMINAL_CSI_END TERMINAL_CSI "4~"
#define TERMINAL_CSI_PGUP TERMINAL_CSI "5~"
#define TERMINAL_CSI_PGDN TERMINAL_CSI "6~"
#define TERMINAL_CSI_HOME TERMINAL_CSI "7~"

#define TERMINAL_CMD_TABLE_BEGIN command_t g_commands[] = {
#define TERMINAL_CMD_TABLE_ENTRY(COMMAND, FUNCTION) { COMMAND, FUNCTION },
#define TERMINAL_CMD_TABLE_END }; const uint32_t g_commands_count = sizeof(g_commands) / sizeof(g_commands[0]);

#define TERMINAL_CMD_BUFFER_SIZE 32
#define TERMINAL_ARG_BUFFER_SIZE 32

typedef struct
{
    char command[TERMINAL_CMD_BUFFER_SIZE];
    void (*function)(uint32_t argc, char** argv);
} command_t;

void terminal_init(void);

#endif