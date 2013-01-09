# Documentation on task 4
-------------------------

## How to run

    $ cc task.c benchmark.c && ./a.out

## How does it work?

This task is implemented rather straightforward. We have created four methods:long calculateTimeElapsed (struct timeval startTime, struct timeval stopTime), long mallocBenchmark (void), long task_allocBenchmark (void) and of course the main method. We will give a short description of those four methods.

### calculateTimeElapsed

This method calculates the difference in time between two timeval structs and converts it to a long. mallocBenchmark and task_allocBenchmark use this method.

### mallocBenchmark

mallocBenchmark requests the current time in microseconds, allocates and deallocates TASK_COUNT (default 100.000) tasks and request the time again. Then, it uses calculatedTimeElapsed to calculate the elapsed time and returns that value.

### task_allocBenchmark

This method work exactly the same as the previous one, except that is uses task_alloc instead of malloc.

### main

The main method calls the benchmark method and prints the results.

## The results

### First test: TASK_COUNT = 100000

In the first test, we have run the benchmarking tool with TASK_COUNT set to 100,000 5 time. The results are:

* malloc: 16667 us, task_alloc: 0 us
* malloc: 16667 us, task_alloc: 0 us
* malloc: 16667 us, task_alloc: 0 us
* malloc: 16667 us, task_alloc: 16667 us
* malloc: 16667 us, task_alloc: 16667 us

It's remarkable that the same number shows up mutiple times, and that task_alloc sometimes takes zero microseconds. We think that this is the case because MINIX updates the time every 16667 us. So, we cannot measure the time the malloc and task_alloc take in microseconds precision. Because these numbers does not say very much (the laptop where this benchmark is run is just to fast:)), we run the test again, but with TASK_COUNT set to 10,000,000.

### First test: TASK_COUNT = 10000000

* malloc: 1000000 us, task_alloc: 300000 us
* malloc: 1000000 us, task_alloc: 283333 us
* malloc: 983334 us, task_alloc: 300000 us
* malloc: 983333 us, task_alloc: 300000 us
* malloc: 1016667 us, task_alloc: 283333 us

One can now clearly see that task_alloc is faster then malloc when setting TASK_COUNT to 10000000.