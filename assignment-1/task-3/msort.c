#include "as1_t2.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
  int start;
  int size;
  task_t** tasks;
} data;


task_t** split_tasks(task_t** tasks, int start, int count){  
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

void *thread_sort(void *args){
  data* thread_data = args;
  
  int start = thread_data->start;
  int size = thread_data->size;
  task_t** tasks = thread_data->tasks;
  
  task_t** result = split_tasks(tasks, start, size);
  msort(result, size);
  
  return result;
}

void msort(task_t** tasks, int count){
  // Base case: 0 or 1 element are already 'sorted'
  if(count<=1){
    return;	
  }

  // Divide input list length in two parts
  int size_left = count / 2;
  int size_right = count - size_left;	
	
	//Left
	data* thread_data_l;
	thread_data_l = malloc(sizeof(data));
  thread_data_l->start = 0;
  thread_data_l->size = size_left;
  thread_data_l->tasks = tasks;
  pthread_t merge_sort_thread_l;  
  task_t** left; 
  void *left_v;
  
  //Right
  data* thread_data_r;
	thread_data_r = malloc(sizeof(data));
  thread_data_r->start = size_left;
  thread_data_r->size = size_right;
  thread_data_r->tasks = tasks;
  //pthread_t merge_sort_thread_r;

  //void *right_v;
 	  task_t** right = thread_sort(thread_data_r);
	if (pthread_create(&merge_sort_thread_l, NULL,  thread_sort, thread_data_l) != 0){
		printf("Error creating left thread!\n");		
	}

	 
//	if (pthread_create(&merge_sort_thread_r, NULL,  thread_sort, thread_data_r) != 0){
	//	printf("Error creating right thread!\n");

	//}	

	pthread_join(merge_sort_thread_l, &left_v);
	//pthread_join(merge_sort_thread_r, &right_v);
	//right = right_v;
	left = left_v;
  // Merge the sorted sublists together
  merge(tasks, left, size_left, right, size_right);
  //free(merge_sort_thread_l);
  //free(merge_sort_thread_r);
}