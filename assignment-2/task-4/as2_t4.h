#ifndef _AS2_T3_H
#define _AS2_T3_H
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

/** In this task, no "MEM_BLOCK_SIZE" or "get_MEM_BLOCK_START()" is provided. You will have to find out the starting address yourself and extend the memory block when necessary. Your virtual machine must have at least 10 MB and at most 512 MB of available memory to pass the test, but you may NOT pre-allocate the entire 10 MB in your code. */

/** You must implement this function. task_alloc allocates memory to task_t elements. */
extern void *task_alloc(void);
/** You must implement this function. task_free frees memory of task_t elements. */
extern void task_free(void *ptr);


#ifdef UNIT_TESTS
void runAllTests(void);
void testAllocUniqueFreeMemory(void);
void testFreeHandlesWrongPointers(void);
void testFreeHandlesNULL(void);
void testReuseFreedMemory(void);
void testFreeUsedMemory(void);
void testRandomFree(void);
#endif /* UNIT_TESTS */

#endif /* _AS2_T3_H */
