#include "patterns.c"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int nums = atoi(argv[1]);
  int pattern = atoi(argv[2]);

  if (pattern == 1) {
    // first pattern
    pattern1(nums);

  } else if (pattern == 2) {
    // second pattern
    pattern2(nums);
  }

  if (pattern == 3) {
    pattern3(nums);
  }
  if (pattern == 4) {
    pattern4(nums);
  }

  return 0;
}