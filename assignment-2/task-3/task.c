#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define MEMORY_BLOCK_SIZE 20

extern void *_sbrk(int);

char *memory_block_start;
char *memory_block_end;

struct linked_list {
  char *first;
  char *last;
  char *current;
} slots;


/* we houden bij
* een linked-list voor vrijgegeven blokken
* het laatste voor-het-eerst-uitgegeven block
* de grenzen van het geheugen
alloceren
- het eerste element van de linked-list wort teruggegeven, en de index wordt een element voorwaards geschoven
- is de linked-list op (je komt op een nil-pointer), dan wordt vanaf het laatst nieuw-vrijgegeven element een nieuw laatst nieuw-vrijgegeven block gegeven
- is de toegewezen ruimte daarbij op, dan wordt er extra geheugenruimte aangevraagd */

void *task_alloc() {
  void *result;

  /* On the first run, or when the end of the linked list is reached */
  if (!slots.first || *(int *)slots.first == 0) {
    
    /* Increase memory if necessary */
    if (!memory_block_start || slots.first + sizeof(task_t) > memory_block_end) {

      /* Offset is the beginning of the memory block */
      char *offset = _sbrk(MEMORY_BLOCK_SIZE);

      if (offset == (char *) -1) {
        printf("Couldn't allocate memory!\n");
        return;
      }

      if (!memory_block_start) {
        memory_block_start = offset;
      }
      
      /* ... */
      slots.first = offset;
      memory_block_end = offset + MEMORY_BLOCK_SIZE;
    }

    result = slots.first;
    slots.first += sizeof(task_t);

  } else {
    printf("Hello.\n");

    result = slots.first;
    *slots.first = *(int *)(slots.first);
  }

  /* Cleanup */

  /* ... */

  return result;
}


/*  vrijgeven
    - als een block wordt vrijgegeven wordt deze aan de
    linked-list op de laatste plaats toegevoegd, waarbij
    deze voormalig-laatste wordt geupdatet met een
    verwijzing naar dit nieuwe block - zelf verwijst 
    hij daarna naar niets */

void task_free(void *freed_task_pointer) {
  if (freed_task_pointer == NULL) return;
  if ((char *)freed_task_pointer < memory_block_start || (char *)freed_task_pointer > memory_block_end) return;

  /* - als een block wordt vrijgegeven wordt deze aan de
  linked-list op de laatste plaats toegevoegd, waarbij
  deze voormalig-laatste wordt geupdatet met een
  verwijzing naar dit nieuwe block */

  /* wordt deze aan de linked-list op de laatste plaats toegevoegd, */
  if (slots.last) {
    slots.last = freed_task_pointer;  
    *(int *)slots.last = (int)freed_task_pointer;

  } else {
    slots.first = freed_task_pointer;
    slots.last = freed_task_pointer;
  }

  /* zelf verwijst hij daarna naar niets */
  *(int *)freed_task_pointer = *(int *)0;

}

int main(int argc, char *argv[]) {
  void *task_a;
  void *task_b;
  void *task_c;
  void *task_d;
  void *task_e;
  void *task_f;

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

  printf("task_b = %d\n", task_b);
  printf("task_e = %d\n", task_e);

  assert(task_e == task_b);

  /* Allocate task_f */
  task_f = task_alloc();

  printf("task_f = %d\n", task_f);

  assert((int)task_d < (int)task_f); 

}