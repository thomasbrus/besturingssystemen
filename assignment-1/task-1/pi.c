#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct {
  long int max_iterations, max_time;
  long double pi;
  int done;
  pthread_mutex_t lock;
} data;

void *calculate(void *args) {
  int i = 0, time_elapsed = 0;
  struct timeval start_time, current_time;
  data* thread_data = args;

  gettimeofday(&start_time, NULL);

  while ((i < thread_data->max_iterations) && (time_elapsed < thread_data->max_time)) {
    pthread_mutex_lock(&(thread_data->lock));
    thread_data->pi += (pow(-1, i) / (2 * i + 1));
    pthread_mutex_unlock(&(thread_data->lock));

    gettimeofday(&current_time, NULL);    
    time_elapsed = ((current_time.tv_sec * 1000000 + current_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec));

    i++;
  }

  thread_data->done = 1;
  return NULL;
}

int main(int argc, char* argv[]) {
  data* thread_data;
  thread_data = malloc(sizeof(data));

  if (argc != 3) {
    printf("Wrong number of arguments (max time, max iterations)\n");
    return -1;
  }
  thread_data->max_time = atoi(argv[1]) * 1000000;
  thread_data->max_iterations = atoi(argv[2]);
  thread_data->done = 0;

  if (pthread_mutex_init(&(thread_data->lock), NULL) != 0) {
    printf("Error creating mutex!\n");
    return -1;
  }

  pthread_t pi_thread;
  if (pthread_create(&pi_thread, NULL, calculate, thread_data) != 0) {
    printf("Error creating thread!\n");
    return -1;
  }

  while (thread_data->done != 1) {
    usleep(100000);
    pthread_mutex_lock(&(thread_data->lock));
    printf("The value of π ≈ %.40Le\n", thread_data->pi * 4);
    pthread_mutex_unlock(&(thread_data->lock));
  }

  pthread_join(pi_thread, NULL);

  return 0;
}