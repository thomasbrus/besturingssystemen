#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long double calculate(int max_iterations) {
  int iteration = 0;
  long double pi = 0;

  while (iteration < max_iterations) {
    pi = pi + (pow(-1, iteration) / (2 * iteration + 1));
    iteration++;
  }

  return 4 * pi;
}

int main() {
  long double pi = calculate(10000);
  printf("The value of π ≈ %.40Le\n", pi);
}