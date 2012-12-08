#include "as2_t1.h"
#include <stdlib.h>
#include <stdio.h>

/* TODO: keep track of free slots in an array (of booleans)
 * Use linear search in task_alloc to find the first free slot
 * Make sure all tests pass
 */

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
  /* Ignore if the pointer is null */
  if (ptr == NULL) return;

  /* Set the memory address pointer to the beginning of the released memory block */
  memory_address = (unsigned long)ptr;
  memory_address -= sizeof(task_t);
}
