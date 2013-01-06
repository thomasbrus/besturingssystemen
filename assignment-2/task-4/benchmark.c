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

int main(int argc, char *argv[]){
	justATest(10000000);
}