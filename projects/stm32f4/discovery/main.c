#include <libopencm3/stm32/rcc.h>

#include "terminal.h"
#include "jtag.h"

int main(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);

	terminal_init();

	while (1)
	{
		printf("%d\r\n", 42);
	}

	return 0;
}