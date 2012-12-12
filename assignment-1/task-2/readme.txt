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
[3] 	Why do we need synchronisation?

===================================================================================================
[1]	How do we run it?
===================================================================================================
	One can not actually run msort.c since it does not have a main function. Also, this programm
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
	