#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "jtag.h"

static void gpio_init(void);
static void timer_init(void);

static void gpio_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(JTAG_PORT_TDI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, JTAG_PIN_TDI);
	gpio_mode_setup(JTAG_PORT_TDO, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, JTAG_PIN_TDO);
	gpio_mode_setup(JTAG_PORT_TCK, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, JTAG_PIN_TCK);
	gpio_mode_setup(JTAG_PORT_TMS, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, JTAG_PIN_TMS);
#if defined(JTAG_PORT_SRST) && defined(JTAG_PIN_SRST)
	gpio_mode_setup(JTAG_PORT_SRST, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, JTAG_PIN_SRST);
#endif
#if defined(JTAG_PORT_TRST) && defined(JTAG_PIN_TRST)
	gpio_mode_setup(JTAG_PORT_TRST, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, JTAG_PIN_TRST);
#endif

	gpio_set_output_options(JTAG_PORT_TDI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, JTAG_PIN_TDI);
	gpio_set_output_options(JTAG_PORT_TCK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, JTAG_PIN_TCK);
	gpio_set_output_options(JTAG_PORT_TMS, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, JTAG_PIN_TMS);
#if defined(JTAG_PORT_SRST) && defined(JTAG_PIN_SRST)
	gpio_set_output_options(JTAG_PORT_SRST, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, JTAG_PIN_SRST);
#endif
#if defined(JTAG_PORT_TRST) && defined(JTAG_PIN_TRST)
	gpio_set_output_options(JTAG_PORT_TRST, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, JTAG_PIN_TRST);
#endif

	gpio_clear(JTAG_PORT_TDI, JTAG_PIN_TDI);
	gpio_clear(JTAG_PORT_TCK, JTAG_PIN_TCK);
	gpio_clear(JTAG_PORT_TMS, JTAG_PIN_TMS);
#if defined(JTAG_PORT_SRST) && defined(JTAG_PIN_SRST)
	gpio_clear(JTAG_PORT_SRST, JTAG_PIN_SRST);
#endif
#if defined(JTAG_PORT_TRST) && defined(JTAG_PIN_TRST)
	gpio_clear(JTAG_PORT_TRST, JTAG_PIN_TRST);
#endif
}

static void timer_init(void)
{

}

void jtag_init(void)
{
	gpio_init();
	timer_init();
}

void jtag_set_tdi(uint8_t value)
{
	if (value)
	{
		gpio_set(JTAG_PORT_TDI, JTAG_PIN_TDI);
	}
	else
	{
		gpio_clear(JTAG_PORT_TDI, JTAG_PIN_TDI);
	}
}

uint8_t jtag_get_tdo(void)
{
	return gpio_get(JTAG_PORT_TDO, JTAG_PIN_TDO) ? 1 : 0;
}

void jtag_set_tck(uint8_t value)
{
	if (value)
	{
		gpio_set(JTAG_PORT_TCK, JTAG_PIN_TCK);
	}
	else
	{
		gpio_clear(JTAG_PORT_TCK, JTAG_PIN_TCK);
	}
}

void jtag_set_tms(uint8_t value)
{
	if (value)
	{
		gpio_set(JTAG_PORT_TMS, JTAG_PIN_TMS);
	}
	else
	{
		gpio_clear(JTAG_PORT_TMS, JTAG_PIN_TMS);
	}
}

#if defined(JTAG_PORT_SRST) && defined(JTAG_PIN_SRST)
void jtag_set_srst(uint8_t value)
{
	if (value)
	{
		gpio_set(JTAG_PORT_SRST, JTAG_PIN_SRST);
	}
	else
	{
		gpio_clear(JTAG_PORT_SRST, JTAG_PIN_SRST);
	}
}
#endif

#if defined(JTAG_PORT_TRST) && defined(JTAG_PIN_TRST)
void jtag_set_trst(uint8_t value)
{
	if (value)
	{
		gpio_set(JTAG_PORT_TRST, JTAG_PIN_TRST);
	}
	else
	{
		gpio_clear(JTAG_PORT_TRST, JTAG_PIN_TRST);
	}
}
#endif

void jtag_transfer(uint16_t length, uint8_t* input, uint8_t* output)
{
	(void)length;
	(void)input;
	(void)output;
}

void jtag_strobe(uint8_t pulses, bool tms, bool tdi)
{
	(void)pulses;
	(void)tms;
	(void)tdi;
}