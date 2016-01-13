#include <stdlib.h>
#include <stdio.h>

void do_math(int *x) {
  *x += 5;
}
 
int main(void) {
  int result = -1, val = 4;
  for (int i = 0; i < 123; i++) {
    do_math(&val);
  }
  printf("Done\n");
  return result;
}
