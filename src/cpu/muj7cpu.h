
#ifndef MUJ7_CPU_H
#define MUJ7_CPU_H

#include <stdbool.h>
#include <stdint.h>

struct RegisterBank
{
    uint8_t a, b, c, f;
    uint8_t pc, sp;
};

struct CPU
{
    uint8_t *ram;
    struct RegisterBank *registers;
    bool halt;
    uint16_t clock_cycles;
};

struct CPU* create_cpu(uint64_t ram_size);
void execute(struct CPU *cpu);

#endif
