#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

typedef struct {
  int max_iterations, max_time;
} boundaries;

long double pi = 0;
int done;

void *calculate(void *args) {
  int i = 0;
  int time_elapsed = 0;
  boundaries *thread_boundaries = args;

  int begin = (int)time(NULL);
  time_t timer;
  
  while ((i < thread_boundaries->max_iterations) && (time_elapsed < thread_boundaries->max_time)) {

    pi = pi + (pow(-1, i) / (2 * i + 1));
    
    timer = time(NULL);
    time_elapsed = ((int)timer - begin);

    i++;
  }

  done = 1;
  return NULL;
}

int main(int argc, char* argv[]) {
  int max_time, max_iterations;

  boundaries* thread_boundaries;
  thread_boundaries = malloc(sizeof(boundaries));

  thread_boundaries->max_time = atoi(argv[1]);
  thread_boundaries->max_iterations = atoi(argv[2]);

  pthread_t pi_thread;
  pthread_create(&pi_thread, NULL, calculate, thread_boundaries);

  int i = 0;
  while (done != 1) {
    usleep(100000);
    printf("The value of π ≈ %.40Le\n", pi * 4);
    i++;
  }

  pthread_join(pi_thread, NULL);

  return 0;
}