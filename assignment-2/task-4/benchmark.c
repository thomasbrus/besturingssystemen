#include "as2_t3.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define TASKS_COUNT 10

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

long mallocBenchmark (int taskCount) {
	time_t startTime;
	time_t stopTime;

	time(&startTime);

	task_t *tasks[taskCount];

	int i;
	for (i = 0; i < taskCount; i++){
		tasks[i] = malloc (sizeof (task_t));
	}

	for (i = 0; i < taskCount; i++) {
		free (tasks (tasks[i]) );
	}

	time(&stopTime);

	return (stopTime - startTime);

}

int main(int argc, char *argv[]){
	long mallocResult;
	mallocResult = mallocBenchmark (TASKS_COUNT);
	printf("Malloc took %d seconds\n", mallocResult);
}