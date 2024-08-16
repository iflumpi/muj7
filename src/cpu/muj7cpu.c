/**
 * @file muj7cpu.h
 *
 * @brief Implementation of muj7cpu structures and functions 
 *
 * @see LICENSE for licensing details.
 */

#include "muj7cpu.h"

#include <stdlib.h>

uint8_t ld(uint8_t *reg_target, uint8_t value)
{
    *reg_target = value;

    return 4;
}

uint8_t add(uint8_t *reg_target, uint8_t value, uint8_t *reg_flags)
{
    uint16_t op_value = *reg_target + value;
    *reg_target = op_value;

    if (op_value > 255)
        *reg_flags |= 0x02;

    return 8;
}

uint8_t sub(uint8_t *reg_target, uint8_t value, uint8_t *reg_flags)
{
    uint8_t op_value = *reg_target - value;
    *reg_target = op_value;

    if (op_value == 0)
        *reg_flags |= 0x01;

    return 8;
}

uint16_t fetch_word_memory(struct CPU *cpu)
{
    uint8_t byte_1 = cpu->ram[cpu->registers->pc++];
    uint8_t byte_2 = cpu->ram[cpu->registers->pc++];
    return (byte_1 << 8) + byte_2;
}

uint16_t get_union(uint8_t reg_1, uint8_t reg_2)
{
    return (reg_1 << 8) + reg_2;
}

struct CPU *create_cpu(uint64_t ram_size)
{
    struct CPU *cpu = (struct CPU *)malloc(sizeof(struct CPU));
    cpu->ram = (uint8_t *)malloc(sizeof(uint8_t) * ram_size);
    cpu->registers = (struct RegisterBank *)malloc(sizeof(struct RegisterBank));

    cpu->registers->pc = 0;
    cpu->registers->sp = 0;
    cpu->registers->a = 0;
    cpu->registers->b = 0;
    cpu->registers->c = 0;
    cpu->registers->f = 0;
    cpu->halt = false;

    return cpu;
}


void execute(struct CPU *cpu)
{
    if (cpu->halt)
        return;

    uint8_t instruction = cpu->ram[cpu->registers->pc++];
    switch (instruction)
    {
    case 0x00: // NOP
        cpu->clock_cycles += 4;
        break;

    case 0x01: // LD B,A
        ld(&cpu->registers->b, cpu->registers->a);
        cpu->clock_cycles += 4;
        break;

    case 0x02: // LD C,A
        ld(&cpu->registers->c, cpu->registers->a);
        cpu->clock_cycles += 4;
        break;

    case 0x03: // LD B,C
        ld(&cpu->registers->b, cpu->registers->c);
        cpu->clock_cycles += 4;
        break;

    case 0x04: // LD A, value
        ld(&cpu->registers->a, cpu->ram[cpu->registers->pc++]);
        cpu->clock_cycles += 4;
        break;

    case 0x05: // ADD B (a <- a+b)
        add(&cpu->registers->a, cpu->registers->b, &cpu->registers->f);
        cpu->clock_cycles += 4;
        break;

    case 0x06: // ADD C (a <- a+c)
        add(&cpu->registers->a, cpu->registers->c, &cpu->registers->f);
        cpu->clock_cycles += 4;
        break;

    case 0x07: // SUB B (a <- a-b)
        sub(&cpu->registers->a, cpu->registers->b, &cpu->registers->f);
        cpu->clock_cycles += 4;
        break;

    case 0x08: // SUB C (a <- a-c)
        sub(&cpu->registers->a, cpu->registers->c, &cpu->registers->f);
        cpu->clock_cycles += 4;
        break;

    case 0x09: // STORE A,[AB]
        cpu->ram[get_union(cpu->registers->a, cpu->registers->b)] = cpu->registers->a;
        cpu->clock_cycles += 4;
        break;

    case 0x0A: // STORE A,[BC]
        cpu->ram[get_union(cpu->registers->b, cpu->registers->c)] = cpu->registers->a;
        cpu->clock_cycles += 4;
        break;

    case 0x0B: // JP *
        cpu->registers->pc = fetch_word_memory(cpu);
        cpu->clock_cycles += 4;
        break;

    case 0x0C: // JPZ *
        if (cpu->registers->f & 0x01)
            cpu->registers->pc = fetch_word_memory(cpu);
        else
            cpu->registers->pc += 2;
        cpu->clock_cycles += 4;
        break;

    case 0x0D: // HALT
        cpu->halt = true;
        break;

    case 0x0E: // INC A
        add(&cpu->registers->a, 1, &cpu->registers->f);
        break;

    case 0x0F: // INC B
        add(&cpu->registers->b, 1, &cpu->registers->f);
        break;

    case 0x10: // INC C
        add(&cpu->registers->c, 1, &cpu->registers->f);
        break;

    case 0x11: // DEC A
        sub(&cpu->registers->a, 1, &cpu->registers->f); 
        break;

    case 0x12: // DEC B
        sub(&cpu->registers->b, 1, &cpu->registers->f); 
        break;

    case 0x13: // DEC C
        sub(&cpu->registers->c, 1, &cpu->registers->f); 
        break;

    default:
        break;
    }
}
