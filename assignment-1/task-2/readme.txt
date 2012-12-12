______               _                                           _         
| ___ \             | |                                         | |        
| |_/ /___  __ _  __| |_ __ ___   ___   _ __ ___  ___  ___  _ __| |_   ___ 
|    // _ \/ _` |/ _` | '_ ` _ \ / _ \ | '_ ` _ \/ __|/ _ \| '__| __| / __|
| |\ \  __/ (_| | (_| | | | | | |  __/ | | | | | \__ \ (_) | |  | |_ | (__ 
\_| \_\___|\__,_|\__,_|_| |_| |_|\___| |_| |_| |_|___/\___/|_|   \__(_)___|
                                                                           
																		   
																		   
===================================================================================================
Contents
===================================================================================================
[1] 	How do we run it?
[2] 	How does the program work?

===================================================================================================
[1]	How do we run it?
===================================================================================================
	One can not actually run msort.c, since it does not have a main function. Also, this program
	is more likely to be used as a part of a library. However, we can run a program to test
	msort.c. First, we need to compile the program. Navigate to the source files with the terminal
	in a UNIX environment with gcc (we used the Ubuntu image) and enter the following command:
	
	gcc as1_t2.c msort.c
	
	This command compiles as1_t2.c, with the ability to refer to functions of msort.c. In order to
	run it, we use the following command:
	
	./a.out
	
===================================================================================================
[2]	How does the program work?
===================================================================================================
	The function to be used is "void msort(task_t** tasks, int count)". This function does
	mergesort in-place on tasks with length count in a recursive manner. The base case is the case
	that count <= 1. Then, there is only one element so the array is already sorted. Else, tasks
	will be divided in two arrays, namely left and right with length size_left and size_right.
	This will be done by calling "task_t** splitTasks(task_t** tasks, int start, int count)" two
	times, which copies a part (between start and start + count) of tasks and returns it.
	
	After splitting tasks in two arrays, msort will be called on them. In this way, tasks will be
	split until the arrays are of length one. Then, the recursion will be unfold be merging the
	arrays. At this time, the actual sorting is done. "void merge(task_t** tasks, task_t**
	tasksleft, int size_left,task_t** tasksright, int size_right)" merges tasksleft and tasksright
	and puts the result in tasks. Eventually, every array is merged in tasks and the sorting has
	been completed.