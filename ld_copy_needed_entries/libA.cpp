#include <stdio.h>

int funB1();

int funA1() {
  printf("in funA1 \n");

  funB1();

  return 0;
}