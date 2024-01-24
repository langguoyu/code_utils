#include <stdio.h>

int funC1();

int funB1() {
  printf("in funB1");
  funC1();

  return 0;
}

int funB2() {
  printf("in funB2");

  return 0;
}