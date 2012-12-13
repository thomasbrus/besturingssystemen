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
	
	$ gcc as1_t2.c msort.c -lpthread
	
	This command compiles as1_t2.c, with the ability to refer to functions of msort.c. Also, we add
	-lpthread because this version of msort uses threads. To run it, we use the following command:
	
	$ ./a.out
	
===================================================================================================
[2]	How does the program work?
===================================================================================================
	This program is almost the same as msort.c in task 2, however it is now thread-friendly build.
	
	This means we have changed the following:
	* structs are used to pass more arguments to the threaded (thus 'merge') method, since we can
	  only pass one argument to a thread
	* to limit the amount of threads created, a counter is updated every time one is created or
	  destroyed and if those limits are reached or it is for some other reason not possible to create
	  a thread we call the method sequentially
