#include "as2_t1.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Reserve the first halve of the available memory for memory management */
int memory_management_offset = 524288;
int memory_slot_count = MAX_TASK_COUNT / 2;

void *task_alloc(void) {
  unsigned long pointer;

  /* Find the first free slot */
  int slot_array_index = 0;
  
  int slot_array_location;
  int *slot_array_pointer;

  while (slot_array_index <= memory_slot_count) {
    slot_array_location = (unsigned long)get_MEM_BLOCK_START() + sizeof(int) * slot_array_index;    
    slot_array_pointer = (int *)slot_array_location;    
    if (*slot_array_pointer == 0) break;
    slot_array_index++;
  }

  if (slot_array_index < memory_slot_count) {
    /* Mark this memory block as taken */    
    /* memset((void *)slot_array_location, 1, sizeof(int)); */
    *slot_array_pointer = (int *)1;
  } else {
    /* All memory blocks are taken */
    return NULL;
  }

  /* Calculate address from the available slot index */
  pointer = (unsigned long)get_MEM_BLOCK_START() + memory_management_offset + (slot_array_index * sizeof(task_t));
  return (void *)pointer;
}

void task_free(void *ptr) {
  int *slot_pointer;
  int slot_location;
  unsigned long slot_index;

  /* Ignore if the pointer is null */
  if (ptr == NULL) return;

  /* Calculate to which slot this pointer belongs */
  slot_index = ((unsigned long)ptr - (unsigned long)get_MEM_BLOCK_START() - memory_management_offset) / sizeof(task_t);

  /* Also ignore if slot is out of bounds */
  if (slot_index < 0 || slot_index >= MAX_TASK_COUNT) return;
  
  slot_location = (unsigned long)get_MEM_BLOCK_START() + sizeof(int) * slot_index;
  slot_pointer = (int *)slot_location;
  *slot_pointer = (int *)0;
}
