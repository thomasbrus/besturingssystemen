#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_TIME_REACHED 42
#define MAX_ITERATIONS_REACHED 1337

typedef struct {
  // Maximum time in microseconds
  int max_time;
  long int max_iterations; 
  long double pi;
  int done;
  // Used for locking on pi
  pthread_mutex_t lock;
} data;

void *calculate(void *args) {
  int i = 0, time_elapsed = 0;
  struct timeval start_time, current_time;
  data* thread_data = args;

  // Request the time of day in seconds and microseconds
  gettimeofday(&start_time, NULL);

  // Continue while max iterations and max time have not been reached
  while ((i < thread_data->max_iterations) && (time_elapsed < thread_data->max_time)) {
    // Lock while calculating pi
    pthread_mutex_lock(&(thread_data->lock));
    // Leibniz formula
    thread_data->pi += (pow(-1, i) / (2 * i + 1));
    pthread_mutex_unlock(&(thread_data->lock));

    // Calculate the time difference in microseconds
    gettimeofday(&current_time, NULL);    
    time_elapsed = ((current_time.tv_sec * 1000000 + current_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec));

    // Increase the counter
    i++;
  }

  thread_data->done = (i >= thread_data->max_time) ? MAX_TIME_REACHED : MAX_ITERATIONS_REACHED;
  return NULL;
}

int main(int argc, char* argv[]) {
  data* thread_data;
  thread_data = malloc(sizeof(data));

  // Check if the number of arguments is correct
  if (argc != 3) {
    printf("Wrong number of arguments (max time, max iterations)\n");
    return -1;
  }

  // The first argument is in seconds, so multiply it
  thread_data->max_time = atoi(argv[1]) * 1000000;
  thread_data->max_iterations = atoi(argv[2]);
  thread_data->done = 0;

  // Check if the initialization succeeded
  if (pthread_mutex_init(&(thread_data->lock), NULL) != 0) {
    printf("Error creating mutex!\n");
    return -1;
  }

  // Check if creating the thread succeeded
  pthread_t pi_thread;
  if (pthread_create(&pi_thread, NULL, calculate, thread_data) != 0) {
    printf("Error creating thread!\n");
    return -1;
  }

  // Wait for thread to die
  while (thread_data->done == 0) {
    usleep(100000);
    pthread_mutex_lock(&(thread_data->lock));
    printf("The value of π ≈ %.40Le\n", thread_data->pi * 4); // Leibniz
    pthread_mutex_unlock(&(thread_data->lock));
  }

  // Join the thread, in this way we wait for the child process to exit
  pthread_join(pi_thread, NULL);

  if (thread_data->done == MAX_TIME_REACHED) {
    printf("Max time reached\n");
  } else if(thread_data->done == MAX_ITERATIONS_REACHED) {
    printf("Max iterations reached\n");
  }

  return 0;
}
