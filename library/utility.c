#include "utility.h"

#include <printf/printf.h>

uint32_t string_count_value(char* string, char value)
{
    uint32_t spaces = 0;

	while (*string)
	{
		if (*string == value)
		{
			spaces++;
		}

        string++;
	}

	return spaces;
}

void string_replace_value(char* string, char value, char replacement)
{
    while (*string)
    {
        if (*string == value)
        {
            *string = replacement;
        }

        string++;
    }
}

void memory_dump_range(uint8_t* source, uint32_t size, uint32_t block)
{
    for (uint32_t i = 0; i < size; i++)
    {
        if (source[i] >= 32 && source[i] < 127)
        {
            printf("%c", source[i]);
        }
        else
        {
            printf(".");
        }

        if ((i > 0) && ((i % (block - 1)) == 0) && (i < (size - 2)))
        {
            printf("\r\n");
        }
    }
}