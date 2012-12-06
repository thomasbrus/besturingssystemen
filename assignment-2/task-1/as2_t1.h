#ifndef _AS2_T1_H
#define _AS2_T1_H
#define UNIT_TESTS

/* This file is prepared for the course Operating Systems/Besturingssystemen, University of Twente, 2011. Students: you may modify this file during development of your solution (e.g. for debugging purposes), BUT you must eventually hand in a WORKING solution with this file UNMODIFIED. */

/** The (arbitrary) structure that represenst some task that must be handled (irrelevant for this assignment). */
typedef struct task_struct {
	/* Some task id */
	unsigned long id;

	/* Some state */
	long state;

	/* Function delegate address */
	void *function;
} task_t;

/** The total available memory block size: 1 MB */
#define MEM_BLOCK_SIZE  1048576UL

/** The starting address of this free block of memory. Use get_MEM_BLOCK_START in your application. */
void* get_MEM_BLOCK_START(void);

/** You must implement this function. task_alloc allocates memory to task_t elements. */
extern void *task_alloc(void);
/** You must implement this function. task_free frees memory of task_t elements. */
extern void task_free(void *ptr);


#ifdef UNIT_TESTS

/** MAX_TASK_COUNT is the theoretical optimal case of memory allocation: 0% overhead. */
#define MAX_TASK_COUNT (MEM_BLOCK_SIZE / sizeof(task_t))

void runAllTests(void);
void testAllocUniqueFreeMemory(void);
void testAllocHandlesOutOfMemory(void);
void testFreeHandlesWrongPointers(void);
void testFreeHandlesNULL(void);
void testReuseFreedMemory(void);
void testFreeUsedMemory(void);
void testRandomFree(void);
#endif /* UNIT_TESTS */

#endif /* _AS2_T1_H */
