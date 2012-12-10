#include "as2_t1.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned long MEMORY_BLOCK_OFFSET = 524288UL;
int memory_management_initialized = 0;

void initialize_memory_management(int count) {
  int i;

  /* Initally, the current node of the linked list is the first one */
  set_current_node(0);

  /* Initialize the nodes of the linked list */
  for (i = 0; i < count; i++) {
    int* next_node_address;
    int* memory_slot_address;

    /* Set which node is next in the chain */
    next_node_address = (int *)(unsigned long)get_MEM_BLOCK_START() + 1 + (2 * i * sizeof(int));
    
    if (i == count - 1) {
      /* The last node should point back to the first one */
      *next_node_address = (int *)0;
    } else {
      *next_node_address = (int *)(i + 1);
    }

    /* Store the index of a free memory slot */
    memory_slot_address = (int *)(unsigned long)get_MEM_BLOCK_START() + 2 + (2 * i * sizeof(int));
    *memory_slot_address = (int *)i;
  }
}

/* Calculates the address of a memory slot from an index */
int calculate_address_from_memory_slot(int index) {
  return (unsigned long)get_MEM_BLOCK_START() + MEMORY_BLOCK_OFFSET + sizeof(task_t) * index;
}

/* Sets the current node of the linked list */
void set_current_node(int index) {
  int* current_node_index = (int *)(get_MEM_BLOCK_START());
  *current_node_index = (int *)index;
}

void *task_alloc(void) {
  int memory_slot;
  int current_node;
  int next_node;

  /* Initialize the linked list on the first run */
  if (memory_management_initialized == 0) {
    initialize_memory_management(1024);
    /* Make sure this isn't ran the next time */
    memory_management_initialized = 1;
  }

  /* 1. Find out which node is the current node in the linked list */
  current_node = *(int *)(get_MEM_BLOCK_START());

  /* 2. Retrieve the index of the next node */
  next_node = *((int *)(get_MEM_BLOCK_START()) + 1 + (2 * current_node * sizeof(int)));

  /* 3. Set current node to this node's next */
  set_current_node(next_node);

  /* 4. Retrieve the index of a free memory slot from this node */
  memory_slot = *((int *)(get_MEM_BLOCK_START()) + 2 + (2 * current_node * sizeof(int)));


  printf("current_node: %d\n", current_node);
  printf("memory_slot: %d\n", memory_slot);
  printf("next_node: %d\n", next_node);

  return (void *)calculate_address_from_memory_slot(memory_slot);
}

void task_free(void *ptr) {
}