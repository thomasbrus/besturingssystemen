#include "as2_t1.h"
#include <stdlib.h>
#include <stdio.h>

int *memory_slots[MAX_TASK_COUNT] = { 0 };

void *task_alloc(void) {
  unsigned long address;

  /* Find the first free slot */
  int slot = 0;
  while (slot <= MAX_TASK_COUNT && (int)memory_slots[slot] == 1) {
    slot++;
  }

  if (slot < MAX_TASK_COUNT) {
    /* Mark this memory block as taken */
    memory_slots[slot] = (int *)1;
  } else {
    /* All memory blocks are taken */
    return NULL;
  }

  /* Calculate address from the available slot index */
  address = (unsigned long)get_MEM_BLOCK_START() + (slot * sizeof(task_t));
  return (void *)address;
}

void task_free(void *ptr) {
  unsigned long slot;

  /* Ignore if the pointer is null */
  if (ptr == NULL) return;

  /* Calculate to which slot this pointer belongs */
  slot = ((unsigned long)ptr - (unsigned long)get_MEM_BLOCK_START()) / sizeof(task_t);

  /* Also ignore if slot is out of bounds */
  if (slot < 0 || slot >= MAX_TASK_COUNT) return;

  memory_slots[slot] = (int *)0;
}
