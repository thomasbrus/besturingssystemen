#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define MEMORY_BLOCK_SIZE 1024 * 1024

extern void *_sbrk(int);

int initialized = 0;

char *memory_block_start;
char *memory_block_end;
char *last_allocated_slot = 0;

struct linked_list {
  char *first;
  char *last;
} free_slots;


void *task_alloc() {
  char *result;

  if (!initialized) {
    /* Allocate MEMORY_BLOCK_SIZE of memory */
    memory_block_start = _sbrk(MEMORY_BLOCK_SIZE);
    memory_block_end = memory_block_start + MEMORY_BLOCK_SIZE;

    if (memory_block_start == (char *)-1) {
      return NULL;
    }

    /* Return a pointer to the beginning of this block */
    result = memory_block_start;
    initialized = 1;

  } else {

    /* Check if the linked list is non-empty */
    if (free_slots.first) {
      int next = *(int *)free_slots.first;
      result = free_slots.first;

      /* Update first if this slot contains a pointer to the next free slot,
         otherwise make this linked list empty */
      if (next) {
        free_slots.first = (char *)next;

      } else {
        free_slots.first = NULL;
      }

    } else {
      /* In case the linked list is empty, just allocate the slot after the
         last allocated slot */
      result = last_allocated_slot + sizeof(task_t);

      /* Increase the size of this memory block if necessary */
      if (result + sizeof(task_t) >= memory_block_end) {
        if (_sbrk(MEMORY_BLOCK_SIZE) == (char *)-1) {
          return NULL;
        }
        memory_block_end += MEMORY_BLOCK_SIZE;
      }

      /* The last freed slot doesn't exist anymore (?) */
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

  /* Check if linked list is non-empty */
  if (free_slots.last) {
    /* Update the pointer of the current last slot */
    *(int *)(free_slots.last) = (int)freed_task_pointer;
    
    /* The new last slot is the slot we just freed */
    free_slots.last = freed_task_pointer;

  } else {

    /* Initialize empty free_slots */
    free_slots.first = free_slots.last = freed_task_pointer;
  }

  /* Empty this slot */
  *(int *)(freed_task_pointer) = 0;
}

void runTests(void) {
  void *task_a;
  void *task_b;
  void *task_c;
  void *task_d;
  void *task_e;
  void *task_f;
  void *task_g;

  printf("Testing task_alloc and task_free\n");

  /* Allocate 4 task_t's */
  printf("- Allocate 4 task_t's\n");
  task_a = task_alloc();
  assert(0 < (int)task_a);
  task_b = task_alloc();
  assert((int)task_a < (int)task_b);
  task_c = task_alloc();
  assert((int)task_b < (int)task_c);
  task_d = task_alloc();
  assert((int)task_c < (int)task_d);
  
  /* Free task a, b and c (in order a, c, b) */
  printf("- Free task a, b and c (in order a, c, b)\n");
  task_free(task_a);
  task_free(task_c);
  task_free(task_b);

  /* Reallocate slot a, b and c */
  printf("- Reallocate slot a, b and c\n");
  task_e = task_alloc();
  assert(task_e == task_a);

  task_f = task_alloc();
  assert(task_f == task_c);

  task_g = task_alloc();
  assert(task_g == task_b);

  /* Free task g twice */
  printf("- Free task g twice\n");
  task_free(task_g);
  task_free(task_g);

  printf("All tests have succeeded.\n");
}

/*int main(int argc, char *argv[]) {
  runTests();
}
*/
