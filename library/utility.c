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

void memory_dump_hex_range(uint8_t* source, uint32_t size, uint32_t block)
{
    uint32_t i = 0;

    while (i < size)
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

        i++;
    }
}

void memory_dump_bin_range(uint8_t* source, uint32_t size)
{
    uint32_t i = 0;
    uint8_t m = 0;

    while (i < size)
    {
        m = 1 << (i % 8);

        printf("%d", (source[i / 8] & m) == m);

        if (m == 128 && i < (size - 1))
        {
            printf("'");
        }

        i++;
    }
}