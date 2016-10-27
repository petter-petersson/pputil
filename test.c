#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int test_passed = 0;
int test_failed = 0;
int checks_run = 0;

#define fail()  return __LINE__

#define check(cond) do { if (!(cond)) fail(); checks_run++;  } while (0)

void test(int (*func)(void), const char *name) {
  int r = func();
  if (r == 0) {
    test_passed++;
  } else {
    test_failed++;
    printf("FAILED: %s (at line %d)\n", name, r);
  }
}
