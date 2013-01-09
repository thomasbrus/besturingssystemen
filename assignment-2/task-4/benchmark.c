#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define TASKS_COUNT 100000

/*This method calculates the time elapsed given the startTime and stopTime as a timeval struct. This method return the time elapsed in microseconds as a long.*/
long calculateTimeElapsed (struct timeval startTime, struct timeval stopTime){
	/*In order to prevent long overflow, it first calculates the elapsed time seconds accurate. Then it adds the time in microseconds accurate.*/
	return (stopTime.tv_sec - startTime.tv_sec) * 1000000 + (stopTime.tv_usec - startTime.tv_usec);
}

/*This method benchmarks malloc by allocating TASK_COUNT tasks and then deallocating them. It returns the time malloc took in microseconds.*/
long mallocBenchmark (void) {
	/*Array with pointers to tasks.*/
	task_t *tasks[TASKS_COUNT];
	struct timeval startTime;
	struct timeval stopTime;
	int i;
	long timeElapsed;

	/*Get the current time in microseconds.*/
	if (gettimeofday(&startTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	/*Allocate TASK_COUNT tasks using malloc and put the pointers in tasks.*/
	for (i = 0; i < TASKS_COUNT; i++){
		tasks[i] = malloc (sizeof (task_t));
		if(tasks[i] == NULL) {
			printf("Out of memory.\n");
			exit(EXIT_FAILURE);
		}
	}

	/*Deallocate the tasks using malloc.*/
	for (i = 0; i < TASKS_COUNT; i++) {
		free (tasks[i]);
	}

	/*Get the current time in microseconds.*/
	if(gettimeofday(&stopTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	/*Calculated how long it took.*/
	timeElapsed = calculateTimeElapsed(startTime, stopTime);

	return timeElapsed;

}

/*This method benchmarks task_alloc by allocating TASK_COUNT tasks and then deallocating them. It returns the time task_alloc took in microseconds.*/
long task_allocBenchmark (void) {
	/*Array with pointers to tasks.*/
	task_t *tasks[TASKS_COUNT];
	struct timeval startTime;
	struct timeval stopTime;
	int i;
	long timeElapsed;

	/*Get the current time in microseconds.*/
	if (gettimeofday(&startTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}
	
	/*Allocate TASK_COUNT tasks using task_alloc.*/
	for (i = 0; i < TASKS_COUNT; i++){
		tasks[i] = task_alloc ();
	}

	/*Deallocate the tasks using task_alloc.*/
	for (i = 0; i < TASKS_COUNT; i++) {
		task_free (tasks[i]);
	}

	/*Get the current time in microseconds.*/
	if(gettimeofday(&stopTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	timeElapsed = calculateTimeElapsed(startTime, stopTime);

	return timeElapsed;
}

int main(int argc, char *argv[]){
	long mallocResult, task_allocResult;

	/*Benchmark malloc.*/
	printf("Benchmarking malloc by allocating and deallocating %d tasks\n", TASKS_COUNT);
	mallocResult = mallocBenchmark ();
	printf("malloc took %d microseconds\n", mallocResult);

	/*Benchmark task_alloc.*/
	printf("Benchmarking task_alloc by allocating and deallocating %d tasks\n", TASKS_COUNT);
	task_allocResult = task_allocBenchmark();
	printf("task_alloc took %d microseconds\n", task_allocResult);
}