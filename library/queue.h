#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

typedef struct
{
	void* buffer;
	uint32_t buffer_size;
	uint32_t value_size;
	uint32_t read_index;
	uint32_t write_index;
} queue_t;

void queue_init(queue_t* queue, void* buffer, uint32_t buffer_size, uint32_t value_size);
void queue_push(queue_t* queue, void* value);
void queue_push_isr(queue_t* queue, void* value);
void* queue_pop(queue_t* queue);
void* queue_pop_isr(queue_t* queue);

#endif