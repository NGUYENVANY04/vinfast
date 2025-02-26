#include "string.h"
#include <stdio.h>
#include <stdlib.h>
void test(char *str)
{
    char *test = "Hello";
    *str = *test;
}
int main()
{
    char *str_dir = "";
    test(str_dir);
    printf("%s", str_dir);
}
