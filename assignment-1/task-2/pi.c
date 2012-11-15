#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define ITERATIONS 1

long double (*step(long double a, long double b, long double t, long double p))[5] {
  long double nextA, nextB, nextT, nextP, pi;
  long double (*result)[5] = malloc(5 * sizeof(long double));
  nextA = (a + b) / 2;
  nextB = sqrt(a * b);
  nextT = t - p * pow(a - nextA, 2);
  nextP = 2 * p;
  pi = pow(a + b, 2) / (4 * t);
  *result[0] = pi;
  *result[1] = nextA;
  *result[2] = nextB;
  *result[3] = nextT;
  *result[4] = nextP;
  return result;
}

long double calculate(long double (**f(long double, long double, long double, long double)), int n) {
  long double pi, a, b, t, p;
  a = 1;
  b = 1 / sqrt(2);
  t = 1 / 4;
  p = 1;

  while (n > 0) {
    long double result = f(a, b, t, p);
    pi = result[0];
    a = result[1];
    b = result[2];
    t = result[3];
    p = result[4];
    // free(result);
    // result = NULL;
    n--;
  }

  return pi;
}

int main() {
  long double pi = calculate(step, ITERATIONS);
  printf("The value of π ≈ %0.10Le\n", pi);
}

