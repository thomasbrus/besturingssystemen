/* 
This file contains the code of the original malloc.c as given in Minix in 
/usr/src/lib/libc/ansi/malloc.c, but with our comments. We made some general comments about the 
working of malloc(), and we added some comments that explain what a specific line of code does.
Our comments start on line 55.
*/

#if     _EM_PSIZE == 2
#define BRKSIZE         1024
#else
#define BRKSIZE         4096
#endif
#define PTRSIZE         ((int) sizeof(void *))
#define Align(x,a)      (((x) + (a - 1)) & ~(a - 1))
#define NextSlot(p)     (* (void **) ((p) - PTRSIZE))
#define NextFree(p)     (* (void **) (p))

/*
* A short explanation of the data structure and algorithms.
* An area returned by malloc() is called a slot. Each slot
* contains the number of bytes requested, but preceeded by
* an extra pointer to the next the slot in memory.
* '_bottom' and '_top' point to the first/last slot.
* More memory is asked for using brk() and appended to top.
* The list of free slots is maintained to keep malloc() fast.
* '_empty' points the the first free slot. Free slots are
* linked together by a pointer at the start of the
* user visable part, so just after the next-slot pointer.
* Free slots are merged together by free().
*/

extern void *_sbrk(int);
extern int _brk(void *);
static void *_bottom, *_top, *_empty;

static int grow(size_t len)
{
 register char *p;

 assert(NextSlot((char *)_top) == 0);
 if ((char *) _top + len < (char *) _top
     || (p = (char *)Align((ptrint)_top + len, BRKSIZE)) < (char *) _top ) {
       errno = ENOMEM;
       return(0);
 }
 if (_brk(p) != 0)
       return(0);
 NextSlot((char *)_top) = p;
 NextSlot(p) = 0;
 free(_top);
 _top = p;
 return 1;
}

/* Malloc basically works as follows:
Malloc works with slots. A slot contains the number of bytes which are requested in malloc(size), 
preceeded by a pointer to the next slot in the memory. On a new malloc call, it hops through the 
slots that are not in use (‘free slots’) to find a slot that is big enough (it should not overlap 
with the next slot). If no such slot is available within the allocated space, it allocates new 
space by calling _sbrk. If such a slot is available, it allocates that space. If the found space was 
bigger than needed, the remaining empty space is still available as a new (smaller) slot for new 
malloc calls, as it will be split apart (this is only done when the remaining space is not too 
small).

The structure of the slot system can be visualised as follows:
 _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _    _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
|  P   |                                 | |  P   |                                |
| NEXT |            D A T A              | | NEXT |            D A T A             |   (...)
|_SLOT_|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _| |_SLOT_|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |
{                   SLOT #1              } {                   SLOT #2             }

The pointer of slot 1 (P NEXT SLOT) points to the start address of the data part of slot 2. The 
pointer of slot 2 in its turn points to the start address of the data part of slot 3. The last 
slot of the chain has a pointer with address 0, which indicates it is the last slot.
A pointer to the first empty slot is stored in variable _empty. The data part of every empty slot 
only contains another pointer to the next empty slot. In this way, malloc can (using function 
NextFree(), which is declared in malloc.c:16) visit all empty slots to check whether there 
is an empty slot available to be used by the caller of malloc.

The following lines contains the original code of malloc.c. We added a lot of extra comment lines 
in order to make more clear what actually happens.
*/


void *  /* returns pointer to allocated area */
malloc(size_t size)
{
 register char *prev, *p, *next, *new; /* register indicates that these variables will be used intensively */
 register unsigned len, ntries;

 if (size == 0)
       return NULL;  /* if size is 0, there is no need to allocate memory */

 for (ntries = 0; ntries < 2; ntries++){ /* the follow code is executed at least once, maybe twice, depending on whether the memory space has to grow or not. */
       if ((len = Align(size, PTRSIZE) + PTRSIZE) < 2 * PTRSIZE) { /* if this happens the maximum amount of memory is reached and we've wrapped around */
               errno = ENOMEM;
               return NULL;
       }
       if (_bottom == 0) { /* initialisation of memory */
               if ((p = _sbrk(2 * PTRSIZE)) == (char *) -1) /* pointer size x 2, one for the pointer slot and another for the next pointer */
                       return NULL; /* if sbrk failed, return NULL */
               p = (char *) Align((ptrint)p, PTRSIZE);
               p += PTRSIZE;
               _top = _bottom = p;
               NextSlot(p) = 0;
       }

       for (prev = 0, p = _empty; p != 0; prev = p, p = NextFree(p)) { /* let p point to first empty slot (_empty). if p points to a zero, break the loop. */
                /* each iteration, hop to the next free slot */  
               next = NextSlot(p);    /* slot next to p */
               new = p + len;  /* slot position of new slot */
               if (new > next || new <= p)  /* if this is true, a slot with size 'len' will not fit in this empty slot. */
                       continue;  /* so, go to next iteration (see malloc.c: 108)*/
                /* if here, the new slot fits */
               if (new + PTRSIZE < next) {  /* if this is true, there is space left which can be used for another malloc call. */
                       /* we will split the previous slot in two parts: a used part (used by us) and an empty part (can be used by another malloc call) */
                       NextSlot(new) = next;  /* after our slot 'new', the first slot will be 'next'. */
                       NextSlot(p) = new;
                       NextFree(new) = NextFree(p);
                       NextFree(p) = new;
               }
               if (prev) /* if 'prev' has been set */
                       NextFree(prev) = NextFree(p); /* let the ‘next pointer’ of the previous visited slot point to the next free slot, which is the first free slot after our just created slot */
               else  /* if 'prev' has not been set */
                       _empty = NextFree(p);  /* let _empty point to first free slot */
               return p;  /* return pointer to allocated space */
       }
        /* if here, a big enough free slot has not been found */
       if (grow(len) == 0)  /* let memory grow, check if successful */
        /* if it was successful, go to the next iteration of the loop (see malloc.c:84) */
               break;  /* if not, stop malloc (go to malloc.c:134) */
               
 }
 assert(ntries != 2);
 return NULL;
}


/*
The next function, free(), makes a previously allocated slot free again. First it looks for the 
first empty slot after the slot that we’ll free: the NextFree() of our slot should point to that 
slot. After that, we’ll set NextFree() of the first empty slot before our slot to our slot. In this 
way, the chain of empty slots is updated.
*/
void
free(void *ptr)
{
 register char *prev, *next;
 char *p = ptr;

 if (p == 0)
       return;

 assert(NextSlot(p) > p);
 for (prev = 0, next = _empty; next != 0; prev = next, next = NextFree(next))
       if (p < next)
               break;
 NextFree(p) = next;
 if (prev)
       NextFree(prev) = p;
 else
       _empty = p;
 if (next) {
       assert(NextSlot(p) <= next);
       if (NextSlot(p) == next) {              /* merge p and next */
               NextSlot(p) = NextSlot(next);
               NextFree(p) = NextFree(next);
       }
 }
 if (prev) {
       assert(NextSlot(prev) <= p);
       if (NextSlot(prev) == p) {              /* merge prev and p */
               NextSlot(prev) = NextSlot(p);
               NextFree(prev) = NextFree(p);
       }
 }
}