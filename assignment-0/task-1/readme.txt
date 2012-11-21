1. Copy /assignment-0/task-1/dummy.c to /usr/src/lib/libc/other/dummy.c

For a method to be included in the library, we place it by convention in the library’s other folder. 

2. Edit file /usr/src/lib/libc/other/Makefile.inc. Add at the end of the file a ‘\’ and add on a newline

  dummy.c

This will make libc use also dummy.c when it’s library is compiled; however you would still run into warnings as dummy() still isn’t anywhere prototyped (‘declared’). This will happen next.

3. Edit file /usr/include/stdlib.h. Add in the prototypes section

  _PROTOTYPE ( void dummy, (void) );

Now, to make it all run:

4. Navigate to /usr/src/. Run the command `make libraries`. Now move to the resource directory and compile as0_t1.c by typing `cc as0_t1.c` and then let it run by typing `./a.out`.