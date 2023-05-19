#include <stdint.h>

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "terminal.h"

void terminal_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);

	gpio_mode_setup(TERMINAL_PORT_TX, GPIO_MODE_AF, GPIO_PUPD_NONE, TERMINAL_PIN_TX);
	gpio_mode_setup(TERMINAL_PORT_RX, GPIO_MODE_AF, GPIO_PUPD_NONE, TERMINAL_PIN_RX);

	gpio_set_output_options(TERMINAL_PORT_RX, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, TERMINAL_PIN_RX);

	gpio_set_af(TERMINAL_PORT_TX, TERMINAL_AF_TX, TERMINAL_PIN_TX);
	gpio_set_af(TERMINAL_PORT_RX, TERMINAL_AF_RX, TERMINAL_PIN_RX);

	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	nvic_enable_irq(NVIC_USART1_IRQ);

	usart_enable_tx_interrupt(USART1);
	usart_enable_rx_interrupt(USART1);

	usart_enable(USART1);
}

void usart1_isr(void)
{
	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) && ((USART_SR(USART1) & USART_SR_TXE) != 0))
	{
		//usart_send(USART1, '#');
	}

	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) && ((USART_SR(USART1) & USART_SR_RXNE) != 0))
	{
		uint8_t data = usart_recv(USART1);
	}
}

void _putchar(char data)
{
    usart_send(USART1, data);
}