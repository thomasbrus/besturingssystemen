#include "as1_t2.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  int start;
  int size;
  task_t** tasks;
} data;

void *splitTasks(void *args) {
  data* data = args;

  printf("yet another process\n");

  int start = data->start;
  int size = data->size;
  task_t** tasks = data->tasks;
  int i;

  // Allocate memory or return NULL.
  task_t** result = (task_t**) malloc(sizeof(task_t*) * size);
  if (NULL == result) return NULL;

  // Generate sublist
  for (i = 0; i < size; i++) {
    result[i] = tasks[start + i];       
  }

  return (void*) result;
}

/* Merge the splitted arays, namely tasksleft and tasksright, and put the result in tasks. */
void merge(task_t** tasks, task_t** tasksleft, int size_left,task_t** tasksright, int size_right){
  // Initialize index pointers for tasks, tasksleft and tasksright
  int p, p1, p2;
  p = p1 = p2 = 0;

  while (p1 < size_left && p2 < size_right) { 
    // Check whether the index pointers p1 and p2 are stil large enough
    if (tasksleft[p1]->id < tasksright[p2]->id) { 
      // The smallest element in left is smaller than the smallest element in right
      tasks[p++] = tasksleft[p1++];
    } else {
      // The smallest element in right is smaller than the smallest element in left
      tasks[p++] = tasksright[p2++];
    }
  }
  while (p1 < size_left) { 
    // The right one is empty; now add everything that is left in the left one
    tasks[p++] = tasksleft[p1++];
  }

  while (p2 < size_right) { 
    // The left one is empty; now add everything that is left in the right one
    tasks[p++] = tasksright[p2++]; 
  }        
}

/* Recursive way to do mergesort on tasks with length count with the use of mutiple threads. */
void msort(task_t** tasks, int count){
  // Base case: 0 or 1 element are already 'sorted'
  if (count <= 1){
    return;	
  }

  // Divide input list length in two parts
  int size_left = count / 2;
  int size_right = count - size_left;
	
  // Make sublist with these sizes
  data* data_l;
  data_l = malloc(sizeof(data));
  data_l->tasks = tasks;
  data_l->start = 0;
  data_l->size = size_left;

  pthread_t merge_sort_l;  
  pthread_create(&merge_sort_l, NULL, splitTasks, data_l);

  data* data_r;
  data_r = malloc(sizeof(data));
  data_r->tasks = tasks;
  data_r->start = size_left;
  data_r->size = size_right;

  pthread_t merge_sort_r;  
  pthread_create(&merge_sort_r, NULL, splitTasks, data_r);

  // Wait for the threads to complete and capture the results
  void* ret = NULL;

  pthread_join(merge_sort_l, &ret);
  task_t** left = (task_t**)ret;
  pthread_join(merge_sort_r, &ret);
  task_t** right = (task_t**)ret;

  // Sort these sublists (with recursion)
  msort(left, size_left);
  msort(right, size_right);
	
  // Merge the sorted sublists together
  merge(tasks, left, size_left, right, size_right);
}