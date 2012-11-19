#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ITERATIONS 1024

long double f(long double x){
  return 4.0L / (1.0L + x * x);
}

long double calculate(long double n) {
  long double a = 0.0L;
  long double b = 1.0L;
  long double h = (b - a) / n;
  int i;
  long double p1, p2, p3, p4;
  long double curSum = 0.0L;
  long double sum = 0.0L;

  p1 = (7.0L * (f(a) + f(b)));
  p2 = 14.0L;
  
  // Summing stuff
  for (i = 4; i <= n - 4; i = i + 4) {
    curSum = curSum + f(a + (i * h));
  }
  
  // 14 * stuff
  p2 = p2 * curSum;

  // Reset sum
  curSum = 0.0L;
  p3 = 32.0L;
  
  // More summing stuff
  for (i = 1; i <= n - 1; i = i + 2) {
    curSum = curSum + f(a + (i * h));
  }
  // 32 * stuff
  p3 = p3 * curSum;

  // Reset sum
  curSum = 0.0L;
  p4 = 12.0L;
  
  // Even more summing stuff
  for (i = 2; i <= n - 2; i = i + 4) {
    curSum = curSum + f(a + (i * h));
  }
  // 12 * stuff
  p4 = p4 * curSum;

  // Reset sum
  curSum = 0.0L;
  sum = ((2.0L * h) / 45.0L) * (p1 + p2 + p3 + p4);
  
  return sum;
}

int main() {
  long double pi = calculate(ITERATIONS);
  printf("The value of π ≈ %.64Le\n", pi);
}