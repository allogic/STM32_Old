#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include "terminal.h"
#include "jtag.h"

static void program_flash(void);

static void program_flash(void)
{
	printf("Programming flash\r\n");
}

int main(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);

	terminal_init();
	jtag_init();

	while (1)
	{
		if (terminal_arg("program"))
		{
			
		}
	}

	return 0;
}