#include "as1_t2.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  task_t** tasks;
  int count;
} data;

void *threadSort(void *args) {
  data* data = args;
  msort(data->tasks, data->count);

  return NULL;
}

/* Merge the splitted arays, namely tasksleft and tasksright, and put the result in tasks. */
void merge(task_t** tasks, int size_left, int size_right) {
  task_t** taskscopy;
  taskscopy = malloc(sizeof(task_t) * (size_left + size_right));

  // Check if allocation was successful
  if(taskscopy == NULL) {
    printf("Out of memory.\n");
    exit(EXIT_FAILURE);
  }

  // Create a copy of the tasks
  int i;
  for (i = 0; i < size_left + size_right; i++) {
    taskscopy[i] = tasks[i];       
  }

  // Initialize index pointers for tasks, tasksleft and tasksright
  int p, p1, p2;
  p = p1 = p2 = 0;

  while (p1 < size_left && p2 < size_right) { 
    // Check whether the index pointers p1 and p2 are stil large enough
    if (taskscopy[p1]->id < taskscopy[size_left + p2]->id) { 
      // The smallest element in left is smaller than the smallest element in right
      tasks[p++] = taskscopy[p1++];
    } else {
      // The smallest element in right is smaller than the smallest element in left
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

int counter = 1;
int max_threads = 2;

/* Recursive way to do mergesort on tasks with length count with the use of mutiple threads. */
void msort(task_t** tasks, int count) {
  // Base case: 0 or 1 element are already 'sorted'
  if (count <= 1){
    return; 
  }
  
  //create data struct for left array
  data* data_l;
  data_l = malloc(sizeof(data));
  if (data_l == NULL) {
    printf("Out of memory.\n");
  }
  data_l->tasks = tasks;
  
  // Create data struct for right array
  data* data_r;
  data_r = malloc(sizeof(data));
  if (data_r == NULL) {
    printf("Out of memory.\n");
  }

  // Divide input list length in two parts
  data_l->count = count / 2;
  data_r->count = count - data_l->count;
  
  data_r->tasks = tasks + data_l->count;
  
  pthread_t merge_sort_l;
  if (counter < max_threads && pthread_create(&merge_sort_l, NULL, threadSort, data_l) == 0) {
    counter++;
    // Call msort via threadSort in a recursive way on data_l
    threadSort(data_r);
	  // We cannot merge before the other thread is done
    if (pthread_join(merge_sort_l, NULL) != 0){
	    printf("Error joining thread");
      exit(-1);
	  }
	  counter--;
  } else {
    threadSort(data_l);
	  threadSort(data_r);
  }
  
  // Merge the sorted sublists together
  merge(tasks, data_l->count, data_r->count);

  free(data_l);
  free(data_r);
}