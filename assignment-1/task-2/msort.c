#include "as1_t2.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Merge the splitted arrays, by replacing tasks */
void merge(task_t** tasks, int size_left, int size_right) {
  task_t** taskscopy;
  taskscopy = malloc(sizeof(task_t) * (size_left + size_right));

  // Check if allocation was successful
  if(taskscopy == NULL) {
    printf("Out of memory.");
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

/* Recursive way to do mergesort on tasks with length count with the use of mutiple threads. */
void msort(task_t** tasks, int count) {
  // Base case: 0 or 1 element are already 'sorted'
  if (count <= 1){
    return; 
  }

  // Divide input list length in two parts
  int size_left, size_right;
  size_left = count / 2;
  size_right = count - size_left;

  // Sort left and right, recursively and in-place
  msort(tasks, size_left);
  msort(tasks + size_left, size_right);

  // Merge the sorted sublists together
  merge(tasks, size_left, size_right);
}
