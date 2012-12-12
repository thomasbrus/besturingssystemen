______               _                                           _         
| ___ \             | |                                         | |        
| |_/ /___  __ _  __| |_ __ ___   ___   _ __ ___  ___  ___  _ __| |_   ___ 
|    // _ \/ _` |/ _` | '_ ` _ \ / _ \ | '_ ` _ \/ __|/ _ \| '__| __| / __|
| |\ \  __/ (_| | (_| | | | | | |  __/ | | | | | \__ \ (_) | |  | |_ | (__ 
\_| \_\___|\__,_|\__,_|_| |_| |_|\___| |_| |_| |_|___/\___/|_|   \__(_)___|
        _ __  __     __  __                   __  
 _    __(_) /_/ /    / /_/ /  _______ ___ ____/ /__
| |/|/ / / __/ _ \  / __/ _ \/ __/ -_) _ `/ _  (_-<
|__,__/_/\__/_//_/  \__/_//_/_/  \__/\_,_/\_,_/___/

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
	
	gcc as1_t2.c msort.c -lpthread
	
	This command compiles as1_t2.c, with the ability to refer to functions of msort.c. Also, we add
	-lpthread because this version of msort uses threads. To run it, we use the following command:
	
	./a.out
	
===================================================================================================
[2]	How does the program work?
===================================================================================================
	This program is almost the same as msort.c in task 2. The only difference is the implementation
	of the function , spliTasks and the use of an function call thread_sort.
	
	When msort is called, the following will be:
	*   define an instantiation of the struct "data"
	*	allocate memory for the data struct
	*	initialize the values of the data struct
	*	create a new thread and let it do thread_sort on the data struct
	
	???