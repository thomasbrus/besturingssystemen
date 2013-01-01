R E A D M E   T A S K . C

===================================================================================================
Contents
===================================================================================================
[1] How do we run it?
[2] How does the program work?

===================================================================================================
[1] How do we run it?
===================================================================================================
  One can not actually run task.c, since it does not have a main function. In addition, the file
  is more likely to be used as a part of a library. However, we can run the provided test program 
  as2_t1.c to test the functions implemented by task.c.
  First, we need to compile the task.c together with the test. Navigate to the source files with 
  the terminal in an UNIX environment with gcc (we used the Minix image) and enter the following 
  command:
  
  $ gcc task.c as2_t1.c -Wall
  
  This command compiles as2_t1.c, with the ability to refer to functions of msort.c. 
  To run it, we use the following command:
  
  $ ./a.out  
  
===================================================================================================
[2] How does the program work?
===================================================================================================
  * Summary: task.c implements the voids *task_alloc(void) and task_free(void *ptr), as defined in 
    header file as2_t1.h. These functions make it possible to allocate and disallocate memory for 
    task_t data structure (see as2_t1.h:8). 
  
  Our memory slot mechanism works with a linked list. Each entry has its own index number. Every 
  entry consists of a 'next node index' and a data part, which hold the task_t struct. When a slot
  is empty, its 'next node index' contains the index of the next empty slot; if a slot is in use, 
  its 'next node index' is set to -1.  
  The memory space that is allocated for our memory slot mechanism, starts with an integer (called 
  'current node') that holds the index of the first free slot (see int get_current_node(void) in 
  task.c:23). If it turns out that the current node equals -1, no free slots are available.


                    
                   <-                           program memory space                            ->
                   ___ ___________________________________________________________________________
  Main structure: | C |                         L I N K E D   L I S T                             |
                  |___|___________________________________________________________________________|
  C = integer that holds 'current node'


                          <-           index 0          -> <-           index 1          ->
                          _______ ________________________ _______ ________________________
  Linked list structure: | N N I |       T A S K _ T      | N N I |       T A S K _ T      |  (...)
                         |_______|________________________|_______|________________________|  
  NNI = integer that holds the 'next node index'
  TASK_T = task_t struct is placed here


  Description of main functions in task.c:

  TASK_ALLOC(void)
  * On the first run, initialize_linked_list() (see task.c:62 and task.c:37) is called. This 
    function sets up the linked list structure as explained and visualised above. Initially, the 
    'current node' points to the first slot (index number 0) and every slot its 'next node index' 
    points to its right neighbour. The 'next node index' of the last points to the first slot 
    (index number 0). 
  * When initialized, the index number of a free slot is obtained using get_current_node() (see 
    task.c:68 and task.c:23). 
  * Using get_next_node_of_node() (see task.c:69 and task.c:27), the 'current node' is updated to a
    new unused slot. After that, the 'next node index' of the selected slot is set to -1 using 
    set_next_node_of_node() (see task.c:75 and task.c:32). 
  * If the current node index equals -1, the function returns NULL (task.c:78). In the other case, 
    the function returns the memory address of the allocated slot. (task.c:81-82)

  TASK_FREE(void *ptr)
  * First off all, the function checks whether the given pointer is valid. The given pointer should
    point to an address that is in our memory range. This is checked in task.c:92-95. In case of an
    invalid pointer, the function stops. 
  * After that, the 'next node index' of the slot is set to the index of 'current node'.(task.c:98)
  * Finally, the 'current node' is set to the index of the given slot. (task.c:101)
  * The slot is now free, and can be allocated again using *task_alloc(void).