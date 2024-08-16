/**
 * @file muj7cpu.h
 *
 * @brief Definition of muj7cpu structures and functions 
 *
 * Emulates the behaviour of muj7 CPU
 *  3 general-purpose 8-bit registers (A, B, C)
 *  flag 8-bit register (F)
 *  program counter 16-bit register (PC)
 *  19 instructions
 *
 * @see LICENSE for licensing details.
 */

#ifndef MUJ7_CPU_H
#define MUJ7_CPU_H

#include <stdbool.h>
#include <stdint.h>

struct RegisterBank
{
    uint8_t a, b, c, f;
    uint16_t pc, sp;
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
