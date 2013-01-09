/* 
This file contains the code of the original _sbrk.c as given in Minix in 
/usr/src/lib/libc/other/_sbrk.c. This file is included because malloc
calls this function. 
*/

#include <lib.h>
#define sbrk    _sbrk
#include <unistd.h>

extern char *_brksize;

PUBLIC char *sbrk(incr)
int incr;
{
 char *newsize, *oldsize;

 oldsize = _brksize;
 newsize = _brksize + incr;
 if ((incr > 0 && newsize < oldsize) || (incr < 0 && newsize > oldsize))
    return( (char *) -1);
 if (brk(newsize) == 0)
    return(oldsize);
 else
    return( (char *) -1);
}