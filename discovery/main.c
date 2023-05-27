#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include <printf/printf.h>

#include "terminal.h"
#include "jtag.h"

static void flash(uint32_t argc, char** argv);

TERMINAL_CMD_TABLE_BEGIN
TERMINAL_CMD_TABLE_ENTRY("flash", flash)
TERMINAL_CMD_TABLE_END

static void flash(uint32_t argc, char** argv)
{
	
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