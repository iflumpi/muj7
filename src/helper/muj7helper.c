#include "muj7helper.h"

#include <stdio.h>

int load_binary(struct CPU *cpu, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return 1;

    char chunk_buffer[20];
    uint8_t chunk_count;
    uint8_t ram_count = 0;
    while ((chunk_count = fread(chunk_buffer, sizeof(char), 20, file)))
    {
        for (ssize_t i = 0; i < chunk_count; i++)
            cpu->ram[ram_count++] = chunk_buffer[i];
    }

    fclose(file);

    return 0;
}
