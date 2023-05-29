#include <stdlib.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include <printf/printf.h>

#include "terminal.h"
#include "jtag.h"
#include "utility.h"

static void jtag(uint32_t argc, char** argv);

TERMINAL_CMD_TABLE_BEGIN
TERMINAL_CMD_TABLE_ENTRY("jtag", jtag)
TERMINAL_CMD_TABLE_END

static void jtag(uint32_t argc, char** argv)
{
	if (argc > 0)
	{
		if (strcmp("mode", argv[1]) == 0)
		{
			{
				uint8_t input[64] = "\xFF\xFF\xFF\xFF\xFF\xFF\x03";
				uint8_t output[64] = "";

				printf("Input:  ");
				memory_dump_bin_range(input, 64);
				printf("\r\n");

				jtag_transfer(50, input, output);

				printf("Output: ");
				memory_dump_bin_range(output, 64);
				printf("\r\n");
			}

			{
				uint8_t input[64] = "\x3C\xE7";
				uint8_t output[64] = "";

				printf("Input:  ");
				memory_dump_bin_range(input, 64);
				printf("\r\n");

				jtag_transfer(16, input, output);

				printf("Output: ");
				memory_dump_bin_range(output, 64);
				printf("\r\n");
			}
		}
	}
}

int main(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);

	terminal_init();
	jtag_init();

	while (1)
	{

	}

	return 0;
}