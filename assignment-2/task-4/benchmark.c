#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define TASKS_COUNT 100000

void justATest (long useconds) {
	time_t startTime;
	time_t stopTime;

	time(&startTime);
	printf("startTime: %d\n", startTime);

	usleep(useconds);

	time(&stopTime);


	printf("stopTime: %d\n", stopTime);
	printf("Time elapsed: %d seconds\n", (stopTime - startTime));
}

long mallocBenchmark (void) {
	task_t *tasks[TASKS_COUNT];
	time_t startTime;
	time_t stopTime;
	int i;

	time(&startTime);

	for (i = 0; i < TASKS_COUNT; i++){
		tasks[i] = malloc (sizeof (task_t));
	}

	for (i = 0; i < TASKS_COUNT; i++) {
		free (tasks[i]);
	}

	time(&stopTime);

	return (stopTime - startTime);

}

int main(int argc, char *argv[]){
	long mallocResult;
	mallocResult = mallocBenchmark ();
	printf("Malloc took %d seconds\n", mallocResult);
}