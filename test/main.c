#include <stdio.h>
#include <string.h>
void handle_result(char *result);
int main()
{
    char s[] = "ssid=dsf&pass=fs&cost=4&id=fsfs";
    handle_result(s);
    handle_result(s);
}
void handle_result(char *result)
{
    static int a = 1;
    a++;
    printf("%d", a);
    /*    char *token;
        int i = 0;
        token = strtok(result, "&");
        while (token != NULL)
        {
            while (token[i++] != '=') { }
            printf("%s\n", &token[i]);
            i = 0;
            token = strtok(NULL, "&");
        }*/
}
