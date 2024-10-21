#include <stdlib.h>
#include <time.h>
int random_slee(int i) {
  // changing seed for each iteration
  srand(time(NULL) + i);

  return rand() % 8 + 1;
}