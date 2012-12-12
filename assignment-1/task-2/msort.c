#include "as1_t2.h"
#include <stdlib.h>

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

/* Recursive way to do mergesort on tasks with length count. */
void msort(task_t** tasks, int count){
  // Base case: 0 or 1 element are already 'sorted'
  if(count<=1){
    return;	
  }

  // Divide input list length in two parts
  int size_left = count / 2;
  int size_right = count - size_left;
	
  // Make sublist with these sizes
  task_t** left = splitTasks(tasks, 0, size_left);
  task_t** right = splitTasks(tasks, size_left, size_right);
	
  // Sort these sublists (with recursion)
  msort(left, size_left);
  msort(right, size_right);
	
  // Merge the sorted sublists together
  merge(tasks, left, size_left, right, size_right);
}