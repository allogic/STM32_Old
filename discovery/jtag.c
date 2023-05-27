#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "jtag.h"

static void periph_init(void);
static void gpio_init(void);
static void timer_init(void);

static void periph_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_TIM2);
}

static void gpio_init(void)
{
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
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

	timer_disable_counter(TIM2);

	timer_set_counter(TIM2, 0);
	timer_set_prescaler(TIM2, 0);
	timer_set_period(TIM2, 1);

	timer_continuous_mode(TIM2);

	timer_disable_oc_output(TIM2, TIM_OC1);
	timer_disable_oc_output(TIM2, TIM_OC2);
	timer_disable_oc_output(TIM2, TIM_OC3);
	timer_disable_oc_output(TIM2, TIM_OC4);

	timer_disable_preload(TIM2);

	timer_enable_counter(TIM2);
}

void jtag_init(void)
{
	periph_init();
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

void jtag_set_frequency(uint16_t value)
{
	timer_disable_counter(TIM2);

	timer_set_counter(TIM2, 0);
	timer_set_prescaler(TIM2, 0);
	timer_set_period(TIM2, 180000000 / (2 * value * 1000));

	timer_enable_counter(TIM2);
}

void jtag_transfer(uint16_t size, uint8_t* input, uint8_t* output)
{
	uint16_t i = 0;

	timer_set_counter(TIM2, 0);

	TIM_SR(TIM2) = ~TIM_SR_UIF;

	while (i < size)
	{
		uint8_t mask = 0x80 >> (i % 8);

		if (input[i / 8] & mask)
		{
			GPIO_BSRR(JTAG_PORT_TDI) = JTAG_PIN_TDI;
		}
		else
		{
			GPIO_BSRR(JTAG_PORT_TDI) = JTAG_PIN_TDI << 16;
		}

		while ((TIM_SR(TIM2) & TIM_SR_UIF) == 0);

		TIM_SR(TIM2) = ~TIM_SR_UIF;
		GPIO_BSRR(JTAG_PORT_TCK) = JTAG_PIN_TCK;

		while ((TIM_SR(TIM2) & TIM_SR_UIF) == 0);

		TIM_SR(TIM2) = ~TIM_SR_UIF;

		if (GPIO_IDR(JTAG_PORT_TDO) & JTAG_PIN_TDO)
		{
			output[i / 8] |= mask;
		}

		GPIO_BSRR(JTAG_PORT_TCK) = JTAG_PIN_TCK << 16;

		i++;
	}
}

bool jtag_strobe(uint8_t pulses, bool tms, bool tdi)
{
	bool tdo = false;

	jtag_set_tms(tms);
	jtag_set_tdi(tdi);

	if (pulses)
	{
		timer_set_counter(TIM2, 0);

		pulses--;

		while (pulses)
		{
			TIM_SR(TIM2) = ~TIM_SR_UIF;
			GPIO_BSRR(JTAG_PORT_TCK) = JTAG_PIN_TCK;

			while ((TIM_SR(TIM2) & TIM_SR_UIF) == 0);

			TIM_SR(TIM2) = ~TIM_SR_UIF;
			GPIO_BSRR(JTAG_PORT_TCK) = JTAG_PIN_TCK << 16;

			while ((TIM_SR(TIM2) & TIM_SR_UIF) == 0);

			pulses--;
		}

		TIM_SR(TIM2) = ~TIM_SR_UIF;
		GPIO_BSRR(JTAG_PORT_TCK) = JTAG_PIN_TCK;

		while ((TIM_SR(TIM2) & TIM_SR_UIF) == 0);

		TIM_SR(TIM2) = ~TIM_SR_UIF;

		tdo = jtag_get_tdo();
		
		GPIO_BSRR(JTAG_PORT_TCK) = JTAG_PIN_TCK << 16;

		while ((TIM_SR(TIM2) & TIM_SR_UIF) == 0);
	}
	else
	{
		tdo = jtag_get_tdo();
	}

	return tdo;
}