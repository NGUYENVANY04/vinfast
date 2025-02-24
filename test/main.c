#include "string.h"
#include <stdio.h>
int main()
{
    char str[100];
    printf("%ld", sizeof(str));
    printf("%ld", strlen(str));
}
