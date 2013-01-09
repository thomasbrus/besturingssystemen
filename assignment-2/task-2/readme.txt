In this task, we explored how malloc works in Minix. 

We used the manual of malloc (command: man malloc) and the source code of malloc (malloc.c:
/usr/src/lib/libc/ansi/malloc.c) to find out how malloc works. Malloc uses _brk (malloc.c:64) and 
_sbrk (malloc.c:100) if the memory needed is beyond the boundaries of the program’s data space.
We also checked the manual of sbrk (command: man sbrk) in order to understand what it does. We
found out by reading sbrk.c (/usr/src/lib/libc/other/_sbrk.c) that _sbrk uses _brk
(/usr/src/lib/libc/other/_brk.c:16) which uses _syscall at line 24 (syscall.c: 
/usr/src/lib/other/syscall.c) which makes use of _sendrec at line 8. _sendrec (_sendrec.s: 
/usr/src/lib/i386/rts/_sendrec.s) gives the assembly instructions for incrementing the boundaries
of the memory.

The manual page of Malloc summarizes what the function mainly does: "it maintains multiple lists
of free blocks according to size, allocating space from the appropriate list". The manual also
tells us that it uses the function sbrk to get more memory from the system when there is no
suitable space already available.

We included malloc.c with additional comments to explain what malloc does. The files _brk.c and
_sbrk.c are also included. 