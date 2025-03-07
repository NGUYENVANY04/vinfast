#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
void test(char *str)
{
    printf("%s", str);

    free(str);
    printf("%s", str);
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
    char *t = " hehe";
    char *h = malloc(4);
    strcpy(h, "he"); // Sao chép nội dung thay vì gán trực tiếp
    test(h);
}
