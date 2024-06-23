
#include "muj7cpu.h"

#include "stdio.h"

int load_binary(struct CPU *cpu, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    char chunk_buffer[20];
    uint8_t chunk_count;
    uint8_t ram_count = 0;
    while ((chunk_count = fread(chunk_buffer, sizeof(char), 20, file)))
    {
        for (ssize_t i = 0; i < chunk_count; i++)
        {
            cpu->ram[ram_count++] = chunk_buffer[i];
        }
    }

    fclose(file);

    return 0;
}

int main(int argc, char **argv)
{
    struct CPU *cpu = create_cpu(1024);

    cpu->registers->a = 2;
    cpu->registers->b = 4;

    load_binary(cpu, "b.bin");

    // cpu->ram[0] = 0x01;

    printf("hello: %d\n", cpu->registers->a);
    for(int i=0; i<20; i++)
        execute(cpu);
    printf("hello: %d\n", cpu->registers->a);
}
