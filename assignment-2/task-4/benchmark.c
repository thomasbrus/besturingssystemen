#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define TASKS_COUNT 100000

long calculateTimeElapsed (struct timeval startTime, struct timeval stopTime){
	return (stopTime.tv_sec - startTime.tv_sec) * 1000000 + (stopTime.tv_usec - startTime.tv_usec);
}

void justATest (long useconds) {
	struct timeval startTime;
	struct timeval stopTime;
	long startTimeus;
	long stopTimeus;
	long timeElapsed;

	printf("justATest with %d microseconds of waiting\n", useconds);

	if (gettimeofday(&startTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	usleep(useconds);

	if(gettimeofday(&stopTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	startTimeus = startTime.tv_sec * 1000000 + startTime.tv_usec;

	printf("startTime in us: %d\n", startTimeus);

	stopTimeus = stopTime.tv_sec * 1000000 + stopTime.tv_usec;

	printf("stopTime in us: %d\n", stopTimeus);

	timeElapsed = calculateTimeElapsed (startTime, stopTime);

	printf("Time elapsed: %d microseconds\n", timeElapsed);
}

long mallocBenchmark (void) {
	task_t *tasks[TASKS_COUNT];
	struct timeval startTime;
	struct timeval stopTime;
	int i;
	long timeElapsed;

	if (gettimeofday(&startTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < TASKS_COUNT; i++){
		tasks[i] = malloc (sizeof (task_t));
		if(tasks[i] == NULL) {
			printf("Out of memory.\n");
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < TASKS_COUNT; i++) {
		free (tasks[i]);
	}

	if(gettimeofday(&stopTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	timeElapsed = calculateTimeElapsed(startTime, stopTime);

	return timeElapsed;

}

long task_allocBenchmark (void) {
	task_t *tasks[TASKS_COUNT];
	struct timeval startTime;
	struct timeval stopTime;
	int i;
	long timeElapsed;

	if (gettimeofday(&startTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < TASKS_COUNT; i++){
		tasks[i] = task_alloc ();
	}

	for (i = 0; i < TASKS_COUNT; i++) {
		task_free (tasks[i]);
	}

	if(gettimeofday(&stopTime, NULL) != 0) {
		printf("Error getting time of day\n");
		exit(EXIT_FAILURE);
	}

	timeElapsed = calculateTimeElapsed(startTime, stopTime);

	return timeElapsed;
}

int main(int argc, char *argv[]){
	long mallocResult, task_allocResult;
	mallocResult = mallocBenchmark ();
	task_allocResult = task_allocBenchmark();

	printf("malloc took %d microseconds\n", mallocResult);
	printf("task_alloc took %d miliseconds\n", task_allocResult);

/*	justATest(2050000);*/
}