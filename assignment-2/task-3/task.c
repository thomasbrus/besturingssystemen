#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define MEMORY_BLOCK_SIZE 1024

extern void *_sbrk(int);

char *memory_block_start;
char *memory_block_end;
char *empty_slot_pointer;

int counter;

void *task_alloc() {
  void *result;
  counter++;

  if (!memory_block_start) {
    memory_block_start = (char *)_sbrk(MEMORY_BLOCK_SIZE);

    if (memory_block_start == (char *) -1) {
      printf("Couldn't allocate memory!\n");
      return;
    }

    empty_slot_pointer = memory_block_start;
    memory_block_end = memory_block_start + MEMORY_BLOCK_SIZE;
  }

  if (counter % (MEMORY_BLOCK_SIZE / sizeof(task_t)) == 0) {

    empty_slot_pointer = _sbrk(MEMORY_BLOCK_SIZE);

    if (empty_slot_pointer == (char *) -1) {
      printf("Couldn't allocate memory!\n");
      return;
    }

    memory_block_end = empty_slot_pointer + MEMORY_BLOCK_SIZE;
    empty_slot_pointer += sizeof(task_t);

  }

  result = empty_slot_pointer;
  empty_slot_pointer += sizeof(task_t);

  return result;
}

void task_free(void *pointer) {
  if (pointer == NULL) return;
  if ((char *)pointer < memory_block_start || (char *)pointer > memory_block_end) return;

  empty_slot_pointer = pointer;
}