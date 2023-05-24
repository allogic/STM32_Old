#include <string.h>

#include <libopencm3/cm3/cortex.h>

#include "queue.h"

void queue_init(queue_t* queue, void* buffer, uint32_t buffer_size, uint32_t value_size)
{
	queue->buffer = buffer;
	queue->buffer_size = buffer_size;
	queue->value_size = value_size;
	queue->read_index = 0;
	queue->write_index = 0;
}

void queue_push(queue_t* queue, void* value)
{
	cm_disable_interrupts();

	memcpy(((char*)queue->buffer) + queue->write_index, value, queue->value_size);

	queue->write_index += queue->value_size;

	if (queue->write_index >= queue->buffer_size)
	{
		queue->write_index = 0;
	}

	cm_enable_interrupts();
}

void queue_push_isr(queue_t* queue, void* value)
{
	memcpy(((char*)queue->buffer) + queue->write_index, value, queue->value_size);

	queue->write_index += queue->value_size;

	if (queue->write_index >= queue->buffer_size)
	{
		queue->write_index = 0;
	}
}

void* queue_pop(queue_t* queue)
{
	void* value = 0;

	cm_disable_interrupts();

	value = ((char*)queue->buffer) + queue->read_index;

	queue->read_index += queue->value_size;

	if (queue->read_index >= queue->buffer_size)
	{
		queue->read_index = 0;
	}

	cm_enable_interrupts();

	return value;
}

void* queue_pop_isr(queue_t* queue)
{
	void* value = 0;

	value = ((char*)queue->buffer) + queue->read_index;

	queue->read_index += queue->value_size;

	if (queue->read_index >= queue->buffer_size)
	{
		queue->read_index = 0;
	}

	return value;
}