#include <stdint.h>

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "queue.h"

#include "terminal.h"

static void periph_init(void);
static void gpio_init(void);
static void usart_init(void);

static char s_char_queue_buffer[TERMINAL_CHAR_QUEUE_SIZE];

static queue_t s_char_queue;

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
	queue_init(&s_char_queue, s_char_queue_buffer, TERMINAL_CHAR_QUEUE_SIZE, sizeof(char));
	usart_init();
}

bool terminal_arg(const char* value)
{
	//usart_disable_rx_interrupt(USART2);

	uint32_t index = queue_begin(&s_char_queue);
	uint32_t end = queue_end(&s_char_queue);

	printf("index:%u end:%u\r\n", index, end);

	while (index != end)
	{
		printf("%u\r\n", index);
		printf("%c", *(char*)queue_peek(&s_char_queue, index));

		index = queue_inc(&s_char_queue, index);
	}

	printf("\r\n");

	//usart_enable_rx_interrupt(USART2);

	return false;
}

void usart2_isr(void)
{
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) && ((USART_SR(USART2) & USART_SR_RXNE) != 0))
	{
		char value = usart_recv(USART2);

		queue_push_isr(&s_char_queue, &value);
	}
}

void _putchar(char value)
{
	usart_send_blocking(USART2, value);
}