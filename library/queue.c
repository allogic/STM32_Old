#include <string.h>

#include <libopencm3/cm3/cortex.h>

#include "queue.h"

void queue_init(queue_t* queue, void* buffer, uint32_t buffer_count, uint32_t value_size)
{
	queue->buffer = buffer;
	queue->buffer_count = buffer_count;
	queue->buffer_size = buffer_count * value_size;
	queue->value_size = value_size;
	queue->read_index = 0;
	queue->read_offset = 0;
	queue->write_index = 0;
	queue->write_offset = 0;
}

void queue_push(queue_t* queue, void* value)
{
	cm_disable_interrupts();

	memcpy(((char*)queue->buffer) + queue->write_offset, value, queue->value_size);

	queue->write_index += 1;
	queue->write_offset += queue->value_size;

	if (queue->write_index >= queue->buffer_count)
	{
		queue->write_index = 0;
		queue->write_offset = 0;
	}

	cm_enable_interrupts();
}

void queue_push_isr(queue_t* queue, void* value)
{
	memcpy(((char*)queue->buffer) + queue->write_offset, value, queue->value_size);

	queue->write_index += 1;
	queue->write_offset += queue->value_size;

	if (queue->write_index >= queue->buffer_count)
	{
		queue->write_index = 0;
		queue->write_offset = 0;
	}
}

void* queue_pop(queue_t* queue)
{
	void* value = 0;

	cm_disable_interrupts();

	if (queue->read_index != queue->write_index)
	{
		value = ((char*)queue->buffer) + queue->read_offset;

		queue->read_index += 1;
		queue->read_offset += queue->value_size;

		if ((queue->read_index >= queue->buffer_count) || (queue->read_offset >= queue->buffer_size))
		{
			queue->read_index = 0;
			queue->read_offset = 0;
		}
	}

	cm_enable_interrupts();

	return value;
}

void* queue_pop_isr(queue_t* queue)
{
	void* value = 0;

	if (queue->read_index != queue->write_index)
	{
		value = ((char*)queue->buffer) + queue->read_offset;

		queue->read_index += 1;
		queue->read_offset += queue->value_size;

		if ((queue->read_index >= queue->buffer_count) || (queue->read_offset >= queue->buffer_size))
		{
			queue->read_index = 0;
			queue->read_offset = 0;
		}
	}

	return value;
}

void queue_flush(queue_t* queue)
{
	cm_disable_interrupts();

	queue->read_index = 0;
	queue->read_offset = 0;

	queue->write_index = 0;
	queue->write_offset = 0;

	cm_enable_interrupts();
}

void queue_flush_isr(queue_t* queue)
{
	queue->read_index = 0;
	queue->read_offset = 0;

	queue->write_index = 0;
	queue->write_offset = 0;
}

uint32_t queue_count(queue_t* queue)
{
	uint32_t count = 0;

	cm_disable_interrupts();

	count = (queue->write_index + queue->buffer_count - queue->read_index) % queue->buffer_count;

	cm_enable_interrupts();

	return count;
}

uint32_t queue_count_isr(queue_t* queue)
{
	uint32_t count = 0;

	count = (queue->write_index + queue->buffer_count - queue->read_index) % queue->buffer_count;

	return count;
}

uint32_t queue_begin(queue_t* queue)
{
	return queue->read_index;
}

uint32_t queue_end(queue_t* queue)
{
	return queue->write_index;
}

uint32_t queue_next(queue_t* queue, uint32_t index)
{
	return (index + 1) % queue->buffer_count;
}

void* queue_peek(queue_t* queue, uint32_t index)
{
	return ((char*)queue->buffer) + (index * queue->value_size);
}