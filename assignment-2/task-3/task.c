#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define MEMORY_BLOCK_SIZE 1024

extern void *_sbrk(int);

int first_run = 1;

char *memory_block_start;
char *memory_block_end;
char *last_allocated_slot = 0;

struct linked_list {
  char *first;
  char *last;
} free_slots;


void *task_alloc() {
  char *result;

  if (first_run) {
    /* Allocate MEMORY_BLOCK_SIZE of memory */
    memory_block_start = _sbrk(MEMORY_BLOCK_SIZE);
    memory_block_end = memory_block_start + MEMORY_BLOCK_SIZE;

    if (memory_block_start == (char *)-1) {
      printf("Couldn't allocate memory!\n");
      return;      
    }

    /* Return a pointer to the beginning of this block */
    result = memory_block_start;
    first_run = 0;

  } else {

    /* Check if the linked list is non-empty */
    if (free_slots.first) {
      int next = *(int *)free_slots.first;
      result = free_slots.first;

      /* Update first if this slot contains a pointer to the next free slot,
         otherwise make this linked list empty */
      if (next == 0) {
        free_slots.first = NULL;
      } else {
        *free_slots.first = next;        
      }

    } else {
      /* In case the linked list is empty, just allocate the slot after the
         last allocated slot */
      result = last_allocated_slot + sizeof(task_t);

      /* The last freed slot doesn't exist anymore */
      free_slots.last = NULL;
    }

  }

  /* Update the last allocated slot */
  if (result > last_allocated_slot) last_allocated_slot = result;

  return result;
}

void task_free(void *freed_task_pointer) {
  /* Check if this pointer is valid */
  if (freed_task_pointer == NULL) return;
  if ((char *)freed_task_pointer < memory_block_start) return;
  if ((char *)freed_task_pointer > memory_block_end) return;

  /* ... */
  if (free_slots.last) {

    /* ... */
    *(int *)free_slots.last = (int)freed_task_pointer;
    free_slots.last = freed_task_pointer;  

  } else {

    /* ... */
    free_slots.first = free_slots.last = freed_task_pointer;
  }

  /* This slot is the last one, so it doesn't have a next free slot */
  freed_task_pointer = NULL;
}

int main(int argc, char *argv[]) {
  void *task_a;
  void *task_b;
  void *task_c;
  void *task_d;
  void *task_e;
  void *task_f;
  void *task_g;
  void *task_h;
  void *task_i;
  void *task_j;
  
  /* Allocate 4 task_t's */
  task_a = task_alloc();
  assert(0 < (int)task_a);
  
  task_b = task_alloc();
  assert((int)task_a < (int)task_b);
  
  task_c = task_alloc();
  assert((int)task_b < (int)task_c);
  
  task_d = task_alloc();
  assert((int)task_c < (int)task_d);
  
  /* Free task_b */
  task_free(task_b);
  
  /* Allocate task_e */
  task_e = task_alloc();
  assert(task_e == task_b);
  
  /* Allocate task_f */
  task_f = task_alloc();
  assert((int)task_d < (int)task_f);
  
  /* Free task f */
  task_free(task_f);
  
  /* Allocate task g */
  task_g = task_alloc();
  
  assert(task_g == task_f);

}