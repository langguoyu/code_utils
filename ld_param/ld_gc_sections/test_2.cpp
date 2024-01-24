#include <stdio.h>
void test_a();
void test_b();
__attribute__((visibility("default"))) void test_2()
{
    printf("test_2\n");
    test_a();
    printf("test_2\n");
}
__attribute__((visibility("default"))) void test_2_1()
{
    printf("test_2_1\n");
}
