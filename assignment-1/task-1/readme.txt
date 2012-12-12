______               _                        _       
| ___ \             | |                      (_)      
| |_/ /___  __ _  __| |_ __ ___   ___   _ __  _   ___ 
|    // _ \/ _` |/ _` | '_ ` _ \ / _ \ | '_ \| | / __|
| |\ \  __/ (_| | (_| | | | | | |  __/ | |_) | || (__ 
\_| \_\___|\__,_|\__,_|_| |_| |_|\___| | .__/|_(_)___|
                                       | |            
                                       |_|            

===================================================================================================
Contents
===================================================================================================
[1] 	How do we run it?
[2] 	How does the program work?
[3] 	Why do we need synchronisation?

===================================================================================================
[1]	How do we run it?
===================================================================================================
	First, we need to compile the program. Navigate to the source file with the terminal in a UNIX
	environment with gcc (we used the Ubuntu image) and enter the following	command:
	
	gcc pi.c -pthread -lm
	
	We need to add -pthread as argument because it's using threads. -lm is needed to use
	mathematical operations such as "double pow(double base, double exp)".
	
	Next, we execute the program with the following command:
	
	./a.out <max_time_in_s> <max_terations>

===================================================================================================
[2]	How does the program work?
===================================================================================================
	This task is rather straightforward. First, we define a struct called data. This struct
	contains all the information needed to approximate pi and is passed as argument to
	"void *calculate(void *args)".
	
	When "int main(int argc, char* argc)" is called, the variables of data are set and a mutex is
	created. Next, a thread is created which will be used to compute pi. This computation is done
	by using the Leibniz formula for pi. Every iteration, one term is added. The current thread
	will wait until the computation is done. Meanwhile, it prints the current approximation of pi.

	The computation is done when max_time or max_iterations is reached.

===================================================================================================
[3]	Why do we need synchronisation?
===================================================================================================
	There are two threads that are accessing long double pi: one for computating pi and one for
	printing the current approximation of pi. We don't want the two thread to both write pi and
	print pi, because it will print an incorrect value of pi. This is possible because a long
	double does not fit in one memory word and thus memory isolation won't help us at this point.
	Also, we don't want that pi is being printed before it's first computation.