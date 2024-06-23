
#include "muj7/muj7cpu.h"

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

// Test case for the ld function
void test_instruction_01(void)
{
    struct CPU *cpu = create_cpu(1024);

    cpu->registers->a = 2;
    cpu->registers->b = 4;
    cpu->ram[0] = 0x01;
    execute(cpu);

    CU_ASSERT(cpu->registers->a == 4)
    CU_ASSERT(cpu->registers->b == 4);
}

// Test case for the ld function
void test_instruction_02(void)
{
    struct CPU *cpu = create_cpu(1024);

    cpu->registers->a = 8;
    cpu->registers->c = 17;
    cpu->ram[0] = 0x02;
    execute(cpu);

    CU_ASSERT(cpu->registers->a == 17)
    CU_ASSERT(cpu->registers->c == 17);
}

// Test case for the ld function
void test_instruction_03(void)
{
    struct CPU *cpu = create_cpu(1024);

    cpu->registers->b = 4;
    cpu->registers->c = 31;
    cpu->ram[0] = 0x03;
    execute(cpu);

    CU_ASSERT(cpu->registers->b == 31)
    CU_ASSERT(cpu->registers->c == 31);
}

// Test case for the ld function
void test_instruction_04(void)
{
    struct CPU *cpu = create_cpu(1024);

    cpu->ram[0] = 0x04;
    cpu->ram[1] = 72;
    execute(cpu);

    CU_ASSERT(cpu->registers->a == 72)
}

int main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        fprintf(stderr, "Initialization of Test Registry failed.");
        return EXIT_FAILURE;
    }

    CU_pSuite suite = CU_add_suite("Suite", init_suite, clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return EXIT_FAILURE;
    }

    CU_add_test(suite, "test_instruction_01", test_instruction_01);
    CU_add_test(suite, "test_instruction_02", test_instruction_02);
    CU_add_test(suite, "test_instruction_03", test_instruction_03);
    CU_add_test(suite, "test_instruction_04", test_instruction_04);

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Cleanup and exit
    CU_cleanup_registry();
    return CU_get_error();
}
