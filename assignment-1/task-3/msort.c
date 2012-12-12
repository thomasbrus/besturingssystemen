#include "as1_t2.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  task_t** tasks;
  int count;
} data;

/* Copy a part of an array to an new array, beginning with start and ending with start + count - 1. */
task_t** splitTasks(task_t** tasks, int start, int count){  
  int i;

  // Allocate memory or return NULL.
  task_t** result = (task_t**) malloc(sizeof(task_t*) * count);
  if (NULL == result) return NULL;

  // Generate sublist
  for (i = 0; i < count; i++) {
    result[i] = tasks[start + i];       
  }
  return result;
}

/* We need an other function that is taking one parameter. ThreadSort just calls msort */
void *threadSort(void *args) {
	data* data = args;
	msort(data->tasks, data->count);
	return;
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
  
  //create data struct for left array
  data* data_l;
  data_l = malloc(sizeof(data));
  
  //create data struct for right array
  data* data_r;
  data_l = malloc(sizeof(data));

  // Divide input list length in two parts
  data_l->count = count / 2;
  data_r->count = count - data_l->count;
  
  // Make sublist with these sizes
  data_l->tasks = splitTasks(tasks, 0, data_l->count);
  data_r->tasks = splitTasks(tasks, data_l->count, data_r->count);
  
  //call msort via threadSort in a recursive way on data_l
  pthread_t merge_sort_l;  
  pthread_create(&merge_sort_l, NULL, threadSort, data_l);
  
  //while merge_sort_l is running, sort the right array in the current thread
  threadSort(data_r);
	
  //we cannot merge before the other thread is done
  pthread_join(merge_sort_l, NULL);
	
  // Merge the sorted sublists together
  merge(tasks, data_l->tasks, data_l->count, data_r->tasks, data_r->count);
}