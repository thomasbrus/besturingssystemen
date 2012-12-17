#include "as1_t2.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>

/* A struct to pass more than argument to pthread */
typedef struct {
  task_t** tasks;
  int count;
} data;

/* Just a wrapper for msort so we can pass one argument to pthread */
void *threadSort(void *args) {
  data* data = args;
  msort(data->tasks, data->count);

  return NULL;
}

/* Merge the splitted arrays, by replacing tasks */
void merge(task_t** tasks, int size_left, int size_right) {
  // Create a deep copy of tasks
  task_t** taskscopy;
  taskscopy = malloc(sizeof(task_t) * (size_left + size_right));

  if(taskscopy == NULL) {
    printf("Out of memory.\n");
    exit(EXIT_FAILURE);
  }

  int i;
  for (i = 0; i < size_left + size_right; i++) {
    taskscopy[i] = tasks[i];       
  }

  // Initialize index pointers for tasks, left and right
  int p, p1, p2;
  p = p1 = p2 = 0;

  while (p1 < size_left && p2 < size_right) { 
    // Check whether the index pointers p1 and p2 are stil large enough
    if (taskscopy[p1]->id < taskscopy[size_left + p2]->id) { 
      // The smallest element in left is smaller than the smallest element in right, copy left
      tasks[p++] = taskscopy[p1++];
    } else {
      // The smallest element in right is smaller than the smallest element in left, copy right
      tasks[p++] = taskscopy[size_left + p2++];
    }
  }
  while (p1 < size_left) { 
    // The right one is empty; now add everything that is left in the left one
    tasks[p++] = taskscopy[p1++];
  }

  while (p2 < size_right) { 
    // The left one is empty; now add everything that is left in the right one
    tasks[p++] = taskscopy[size_left + p2++]; 
  }

  free(taskscopy);
}

int thread_counter = 1;

/* Recursive way to do mergesort on tasks with length count with the use of mutiple threads. */
void msort(task_t** tasks, int count) {
  // Base case: 0 or 1 element are already 'sorted'
  if (count <= 1){
    return; 
  }
  
  // Create data struct for left array
  data* data_l;
  data_l = malloc(sizeof(data));
  if (data_l == NULL) {
    printf("Out of memory.\n");
    exit(EXIT_FAILURE);
  }
  
  // Create data struct for right array
  data* data_r;
  data_r = malloc(sizeof(data));
  if (data_r == NULL) {
    printf("Out of memory.\n");

    free(data_l);
    exit(EXIT_FAILURE);
  }

  // Divide input list length in two parts
  data_l->count = count / 2;
  data_r->count = count - data_l->count;

  data_l->tasks = tasks;

  // Move pointer a half length ahead
  data_r->tasks = tasks + data_l->count;
  
  pthread_t merge_sort_l;
  int max_threads = 4;
  // To make use of all available cores (and not the fixed number above), you could uncomment the line below
  // however, the lookup routine is taking so much time that it slows down the process significantly, so this is not recommended
  // int max_threads = get_nprocs();

  if (thread_counter < max_threads && pthread_create(&merge_sort_l, NULL, threadSort, data_l) == 0) {
    thread_counter++;
    // Call msort via threadSort in a recursive way on data_l
    threadSort(data_r);
	  // We cannot merge before the other thread is done
    if (pthread_join(merge_sort_l, NULL) != 0) {
	    printf("Error joining thread.\n");
      exit(EXIT_FAILURE);
	  }

	  thread_counter--;
  } else {
    // if it is impossible to create a thread, run it sequentially
    threadSort(data_l);
	  threadSort(data_r);
  }
  
  // Merge the sorted sublists together
  merge(tasks, data_l->count, data_r->count);

  free(data_l);
  free(data_r);
}