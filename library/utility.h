#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>

uint32_t string_count_value(char* string, char value);
void string_replace_value(char* string, char value, char replacement);

void memory_dump_hex_range(uint8_t* source, uint32_t size, uint32_t block);
void memory_dump_bin_range(uint8_t* source, uint32_t size);

#endif