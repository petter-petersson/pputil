#ifndef __TEST_H
#define __TEST_H

int test_passed;
int test_failed;
int checks_run;

#define fail()  return __LINE__

#define check(cond) do { if (!(cond)) fail(); checks_run++;  } while (0)

void test(int (*func)(void), const char *name);

#endif
