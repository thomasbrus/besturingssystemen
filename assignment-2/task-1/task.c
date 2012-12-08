#include "as2_t1.h"
#include <stdlib.h>
#include <stdio.h>

unsigned long memory_address;

void *task_alloc(void) {
  if (memory_address) {
    /* Increase the pointer by the size of one task_t */
    memory_address += sizeof(task_t);
  } else {
    /* Set memory pointer to the start of the block */
    memory_address = (unsigned long)get_MEM_BLOCK_START();
  }

  /* Check if there's enough space left */
  if (memory_address + sizeof(task_t) >= ((unsigned long)get_MEM_BLOCK_START() + MEM_BLOCK_SIZE)) {
    return NULL;
  }

  return (void *)memory_address;
}

void task_free(void *ptr) {
}
