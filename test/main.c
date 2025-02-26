#include "string.h"
#include <stdio.h>
#include <stdlib.h>
void test(char *str) { printf("%s", str); }
int main()
{
    char str_dir[] = "Hello";

    char *str_test = (char *)malloc(sizeof(str_dir));
    str_test = str_dir;
    strcpy(str_test, str_dir);
    char str[100];
    printf("%s\n", str_test);
    printf("%p\n", &str);
    printf("%p", &str[0]);
    C
    test(str_test);
}
