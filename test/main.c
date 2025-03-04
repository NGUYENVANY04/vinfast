#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
void test(char **str)
{
    char *test = "Hello";
    *str = test;
}
int main()
{
    /*  char *str_dir = "";
      printf("%p\n", str_dir);
      char he[10] = "he";
      printf("%p\n", he);

      str_dir = he;

      printf("%p\n", str_dir);

      test(&str_dir);
      printf("%p\n", str_dir);
      printf("%s\n", str_dir);
      uint32_t number = 12345;
      char buffer[12]; // Đủ lớn để chứa số + ký tự null

      sprintf(buffer, "%u", number);
      printf("Chuỗi: %s\n", buffer);
  */
    char *he;
    printf("%s\n", he);
    char *test = "hello";
    printf("%p\n", test);
    test = "hi";
    int a = 10;
    int *pA = &a;
    printf("%p\n", pA);
    *pA = 5;
    printf("%p\n", pA);

    printf("%p\n", test);
    char str[10];
    printf("%p\n", str);
    printf("%p\n", &str);
}
