#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handler_free(char **ptr)
{
    *ptr = malloc(4 * sizeof(char));
    *ptr = "10";
}

int main(void)
{
    char *str = NULL;
    printf("%s", str);
    printf("%p \n", &(*str));

    handler_free(&str);
    printf("\n%s", str);
    printf("\n%p", &(*str));
    return 0;
}
