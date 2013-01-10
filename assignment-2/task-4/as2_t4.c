#include "as2_t3.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h> /* for doing srand(time(NULL))*/

/* This file is prepared for the course Operating Systems/Besturingssystemen, University of Twente, 2011. Students: you may modify this file during development of your solution (e.g. for debugging purposes), BUT you must eventually hand in a WORKING solution with this file UNMODIFIED. */

#ifdef UNIT_TESTS
/** FAIR_TASK_COUNT tasks should easily fit in the memory slab: 10 MB for slots! */
#define FAIR_TASK_COUNT (10485760UL / sizeof(task_t))

/** Our test keeps track of all allocated task_t objects. */
task_t* tasks[FAIR_TASK_COUNT];

/** The test program */
int main(int argc, char *argv[]){
	printf("= as2_t3 =\n");
	printf("This unit test program is provided for Assignment 2, Task 3 of the course Operating Systems, University of Twente, 2011.\n\n");
	printf("Your solution will be tested on a number of points. Afterwards, \"[End of Test]\" will be shown. IF this is not so, the program crashed and you're not done yet.\n");
	printf("Please note: this program may detect some flaws, but may not detect all of them. Do *not* assume correctness of your solution on passing this test alone.\n\n");


	printf(" || task_t Slot Size              || %10u Bytes ||\n", sizeof(task_t));
	printf(" || task_t Fair Slot Count        || %10lu slots ||\n", FAIR_TASK_COUNT);
	printf("\n");
	
	runAllTests();
}

/** Run all tests, in a predefined order. */
void runAllTests(void){
	printf("[Begin of Test]\n");
	testAllocUniqueFreeMemory();
	testFreeHandlesWrongPointers();
	testFreeHandlesNULL();
	testReuseFreedMemory();
	testFreeUsedMemory();
	testAllocUniqueFreeMemory();
	testRandomFree();
	printf("[End of Test]\n");
}

void testAllocUniqueFreeMemory(void){
	int i;
	task_t *prev_task = NULL;

	printf(" => testAllocUniqueFreeMemory\n");

	for(i = 0; i < FAIR_TASK_COUNT; i++){
		/* Point test slot i to the actual task_alloc() result. */
		tasks[i] = task_alloc();
		/* Slots must be allocated */
		assert(tasks[i] != NULL);
		/* Cannot assert: Slots must be in the allocated space */

		/* Try and set some data */
		tasks[i]->id = i;
		
		/* Compare and set prev_task */
		if (prev_task != NULL){
			/* Slots must be unique in pointer reference */
			assert(tasks[i] != prev_task);
			/* Slots must thus be unique in value */
			assert(tasks[i]->id != prev_task->id);
		}
		prev_task = tasks[i];
		/* Keep track of the amount of tasks*/
	}
}
void testFreeHandlesWrongPointers(void){

	printf(" => testFreeHandlesWrongPointers\n");

	/* This should simply succeed. Pointers to out of bound addresses must be ignored. */	
	task_free((task_t*) 0xFFFFFFFF); /* Pointing to address at 4 GB of memory */
}
void testReuseFreedMemory(void){
	task_t* task;

	printf(" => testReuseFreedMemory\n");

	/* Integrity check of the test. */
	assert(tasks[1] != NULL);

	/* All slots are taken (due to testAllocHandlesOutOfMemory). So we free one and allocate one, which should be identical to that slot. */
	task_free(tasks[1]);
	task = task_alloc();
	/* Freed and reassigned slot identical in pointer. */
	assert(task == tasks[1]); 
	/* Freed and reassigned slot identical in value. */
	assert(task->id == tasks[1]->id);
}

void testFreeUsedMemory(void){
	int i;
	printf(" => testFreeUsedMemory\n");

	/* Also, if we first free FAIR_TASK_COUNT... */
	for(i = 0; i < FAIR_TASK_COUNT; i++){
		/* Skip the entry we used in the previous test. This prevents a double free. */
		if(i==1) continue;
		/* All slots were allocated up to FAIR_TASK_COUNT. */
		assert(tasks[i] != NULL);

		/* Free the task. */
		task_free(tasks[i]);
	}

	/* ...and then alloc FAIR_TASK_COUNT, we should not go out of memory. */
	for(i = 0; i < FAIR_TASK_COUNT; i++){
		/* Allocate a slot */
		tasks[i] = NULL;
		tasks[i] = task_alloc();

		/* The allocation should have succeeded. */
		assert(tasks[i] != NULL);
	}

}
void testFreeHandlesNULL(void){

	printf(" => testFreeHandlesNULL\n");
	
	/* This should simply succeed. Pointers to NULL-address must be ignored. */	
	task_free(NULL);
}
/* We didn't declare this in the .h since we thought it might be confusing. */
void doFree(long i){
	/* It was allocated. */
	assert(tasks[i] != NULL);
	/* Free the task. */
	task_free(tasks[i]);
}
void testRandomFree(void){
	long x,y,a;
	int i;
	
	printf(" => testRandomFree\n");
	
	srand(-1);
	a=FAIR_TASK_COUNT;
	x=rand(); x=((x)%a+a)%a;x=(x==0)?x+1:x;	/*random x s.t. 0<x<a*/
	y=rand(); y=((y)%a+a)%a;y=(y==0)?y+1:y;	/*random y s.t. 0<y<a*/
	if(x>y){long z=x; x=y;y=z;}		/*change x and y s.t. 0<=x<=y<a*/
	if(x+1==y)				/*make sure that x<y-1 to avoid double free*/
	{
		if(x<=1)
			y++;
		else
			x--;
	}
	/*
	We will free x, then y, then 0 to x, then x+1 to y, then y+1 to end.
	If you need to see which x and y are chosen for debugging reasons, just uncomment the prints
	*/
	
	/*printf("Freeing: %d,",x);*/
	doFree(x);
	/*printf(" then %d",y);*/
	doFree(y);
	/*printf(" and then 0 up to %d, and then %d up to %d, and then %d up to the end in sequence.\n", x, x+1,y,y+1);*/
	/* Free 0 up to x */
	for(i = 0; i < x; i++){ doFree(i); } 
	/* Free x+1 up to y */
	for(i = x+1; i < y; i++){ doFree(i); }
	/* Free y up to FAIR_TASK_COUNT */
	for(i = y+1; i < a; i++){ doFree(i); }
}
#endif
