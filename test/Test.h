#ifndef SHRN_TEST_H
#define SHRN_TEST_H

int Indent = 0;

#define SHRN_TEST_INIT() \
    struct { unsigned long TestCount; unsigned long PassCount; } * TestStack = NULL;\
    size_t TestStackSize = 0;\
    unsigned long * TestStackTopTestCount = NULL;\
    unsigned long * TestStackTopPassCount = NULL;\

#define SHRN_TEST_GROUP(group, tests) \
    {\
        int sub = 0;\
        puts("");\
        int i = 0;\
        for (i = 0; i < Indent; i++) printf("    ");\
        Indent++;\
        TestStackSize++;\
        TestStack = SHRN_REALLOC(TestStack, TestStackSize * sizeof(unsigned long) * 2);\
        TestStackTopTestCount = &TestStack[TestStackSize - 1].TestCount;\
        TestStackTopPassCount = &TestStack[TestStackSize - 1].PassCount;\
        *TestStackTopTestCount = 0;\
        *TestStackTopPassCount = 0;\
        printf("Testing group %s...\n\n", #group);\
        tests\
        Indent--;\
        puts("");\
        for (i = 0; i < Indent; i++) printf("    ");\
        printf("%lu/%lu tests passed in group %s.\n", *TestStackTopPassCount, *TestStackTopTestCount, #group);\
        puts("");\
        if (TestStackSize > 1)\
        {\
            TestStackSize--;\
            TestStack[TestStackSize - 1].TestCount += TestStack[TestStackSize].TestCount;\
            TestStack[TestStackSize - 1].PassCount += TestStack[TestStackSize].PassCount;\
            TestStack = SHRN_REALLOC(TestStack, TestStackSize * sizeof(unsigned long) * 2);\
            TestStackTopTestCount = &TestStack[TestStackSize - 1].TestCount;\
            TestStackTopPassCount = &TestStack[TestStackSize - 1].PassCount;\
        }\
    }

#define SHRN_TEST(expr) \
    for (i = 0; i < Indent; i++) printf("    ");\
    if (expr)\
    {\
        printf("#%lu - Passed\n", ++(*TestStackTopTestCount));\
        (*TestStackTopPassCount)++;\
    }\
    else\
    {\
        printf("#%lu - `%s` failed at %s:%i\n", ++(*TestStackTopTestCount), #expr, __FILE__, __LINE__);\
    }

#endif
