#include <stdio.h>

// This is the function we want to call from LLVM
int test_tiny_fn(int x);

int main(int argc, char *argv[]) {
  int i = 1;
  printf("Calling test_tiny_fn(%d)\n", i);
  test_tiny_fn(i);
  printf("Returned from test_tiny_fn(%d)\n", i);
  return 0;
}
