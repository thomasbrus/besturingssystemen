#include "as2_t1.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Utilize 75% of the available space */
long TASK_COUNT = 65535;
int linked_list_initialized = 0;

unsigned long calculate_node_address_from_index(int node_index) {
  return (unsigned long)get_MEM_BLOCK_START() + sizeof(int) + node_index * (sizeof(task_t) + sizeof(int));
}

int calculate_index_from_node_address(unsigned long node_address) {
  return (node_address - (unsigned long)get_MEM_BLOCK_START()) / (sizeof(task_t) + sizeof(int));
}

void set_current_node(int current_node_index) {
  int* current_node_pointer = (int *)(get_MEM_BLOCK_START());
  *current_node_pointer = current_node_index;
}

int get_current_node(void) {
  return *(int *)(get_MEM_BLOCK_START());
}

int get_next_node_of_node(int node_index) {
 return *(int *)calculate_node_address_from_index(node_index);
}

/* Sets the next node for a specific node */
void set_next_node_of_node(int node_index, int next_node_index) {
  int* next_node_pointer = (int *)calculate_node_address_from_index(node_index);
  *next_node_pointer = next_node_index;
}

void initialize_linked_list(int count) {
  int i;

  /* Initially, the current node of the linked list is the first one */
  set_current_node(0);
  assert(get_current_node() == 0);

  /* Initialize the nodes of the linked list */
  for (i = 0; i < count; i++) {
    if (i == count - 1) {
      /* The last node of the linked list points back to first one */
      set_next_node_of_node(i, 0);
    } else {
      set_next_node_of_node(i, i + 1);
    }
  }
}

void *task_alloc(void) {
  int current_node_index;
  int next_node_index;
  int task_t_address;

  /* Initialize the linked list on the first run */
  if (linked_list_initialized == 0) {
    initialize_linked_list(TASK_COUNT);
    /* Make sure this is only ran once */
    linked_list_initialized = 1;
  }

  /* Retrieve both the current node and its next node */
  current_node_index = get_current_node();
  next_node_index = get_next_node_of_node(current_node_index);

  /* Update the current node of the linked list */
  set_current_node(next_node_index);

  /* Make clear this node is already allocated */
  set_next_node_of_node(current_node_index, -1);

  /* The last node of the chain had no next node. This means all memory is allocated */
  if (current_node_index == -1) return NULL;

  /* Calculate the actual address of the allocated task_t */
  task_t_address = calculate_node_address_from_index(current_node_index) + sizeof(int);
  return (void *)task_t_address;
}

void task_free(void *ptr) {
  int node_index;

  /* Ignore invalid pointers */
  if (ptr == NULL) return;

  /* Calculate to which block this pointer belongs */
  node_index = calculate_index_from_node_address((unsigned long)ptr);

  /* Also ignore out of bound pointers */
  if (node_index < 0 || node_index >= TASK_COUNT) return;

  /* -1 indicated that the slot is in use, so let's fix that */
  set_next_node_of_node(node_index, get_current_node());

  /* By updating the linked list's current node, we make sure this slot can be allocated again */
  set_current_node(node_index);
}