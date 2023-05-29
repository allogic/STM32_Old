#include <stdbool.h>
#include <string.h>

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include <printf/printf.h>

#include "terminal.h"
#include "utility.h"

static void periph_init(void);
static void gpio_init(void);
static void usart_init(void);

static void terminal_handle_ansi(uint8_t byte);
static void terminal_execute_buffer(void);

extern command_t g_commands[];
extern uint32_t g_commands_count;

static uint8_t s_cmd_buffer[TERMINAL_CMD_BUFFER_SIZE];
static uint32_t s_arg_buffer[TERMINAL_ARG_BUFFER_SIZE];

static uint32_t s_cursor_position;

static bool s_esc;
static bool s_csi;

static void periph_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART2);
}

static void gpio_init(void)
{
	gpio_mode_setup(TERMINAL_PORT_TX, GPIO_MODE_AF, GPIO_PUPD_NONE, TERMINAL_PIN_TX);
	gpio_mode_setup(TERMINAL_PORT_RX, GPIO_MODE_AF, GPIO_PUPD_NONE, TERMINAL_PIN_RX);

	gpio_set_output_options(TERMINAL_PORT_RX, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, TERMINAL_PIN_RX);

	gpio_set_af(TERMINAL_PORT_TX, TERMINAL_AF_TX, TERMINAL_PIN_TX);
	gpio_set_af(TERMINAL_PORT_RX, TERMINAL_AF_RX, TERMINAL_PIN_RX);
}

static void usart_init(void)
{
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	nvic_enable_irq(NVIC_USART2_IRQ);

	usart_enable_rx_interrupt(USART2);

	usart_enable(USART2);
}

void terminal_init(void)
{
	periph_init();
	gpio_init();
	usart_init();
}

static void terminal_handle_ansi(uint8_t byte)
{
	if (s_esc)
	{
		if (s_csi)
		{
			if (byte == 65) // Move up
			{
				s_esc = false;
				s_csi = false;
			}
			else if (byte == 66) // Move down
			{
				s_esc = false;
				s_csi = false;
			}
			else if (byte == 67) // Move back
			{
				if (s_cursor_position < (TERMINAL_CMD_BUFFER_SIZE - 1))
				{
					s_cursor_position++;
				
					printf(TERMINAL_CSI_CUF(1));
				}

				s_esc = false;
				s_csi = false;
			}
			else if (byte == 68) // Move forward
			{
				if (s_cursor_position > 0)
				{
					s_cursor_position--;
				
					printf(TERMINAL_CSI_CUB(1));
				}

				s_esc = false;
				s_csi = false;
			}
			else
			{
				s_esc = false;
				s_csi = false;
			}
		}
		else
		{
			if (byte == 91) // CSI mode
			{
				s_csi = true;
			}
			else
			{
				s_esc = false;
			}
		}
	}
	else
	{
		if (byte == 8 || byte == 127) // Backspace & Delete
		{
			if (s_cursor_position > 0)
			{
				s_cursor_position--;

				s_cmd_buffer[s_cursor_position] = 0;

				printf(TERMINAL_CSI_CUB(1) " " TERMINAL_CSI_CUB(1));
			}
		}
		else if (byte == 13) // Return
		{
			s_cursor_position = 0;
			
			printf("\r\n");

			terminal_execute_buffer();
		}
		else if (byte == 27) // ESC mode
		{
			s_esc = true;
		}
		else if (byte >= 32 && byte < 127)
		{
			if (s_cursor_position < (TERMINAL_CMD_BUFFER_SIZE - 1))
			{
				s_cmd_buffer[s_cursor_position] = byte;

				s_cursor_position++;

				printf("%c", byte);
			}
		}
	}
}

static void terminal_execute_buffer(void)
{
	for (uint32_t i = 0; i < g_commands_count; i++)
	{
		if (memcmp(s_cmd_buffer, g_commands[i].command, strlen(g_commands[i].command)) == 0)
		{
			uint32_t argc = string_count_value((char*)s_cmd_buffer, 32);

			string_replace_value((char*)s_cmd_buffer, 32, 0);

			s_arg_buffer[0] = (uint32_t)s_cmd_buffer;

			for (uint32_t j = 0, k = 1; j < TERMINAL_CMD_BUFFER_SIZE; j++)
			{
				if ((s_cmd_buffer[j] == 0) && (j < (TERMINAL_CMD_BUFFER_SIZE - 1)) && (s_cmd_buffer[j + 1] != 0))
				{
					s_arg_buffer[k++] = (uint32_t)&s_cmd_buffer[j + 1];
				}
			}

			g_commands[i].function(argc, (char**)s_arg_buffer);

			break;
		}
	}

	memset(s_cmd_buffer, 0, sizeof(s_cmd_buffer));
	memset(s_arg_buffer, 0, sizeof(s_arg_buffer));
}

void usart2_isr(void)
{
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) && ((USART_SR(USART2) & USART_SR_RXNE) != 0))
	{
		uint8_t byte = usart_recv(USART2);

		terminal_handle_ansi(byte);
	}
}

void _putchar(char byte)
{
	usart_send_blocking(USART2, byte);
}