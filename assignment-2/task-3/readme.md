# Documentation on task 3
-------------------------

## How to run

    $ cc task.c as2_t3.c && ./a.out

Also, we provided [additional tests](https://github.com/thomasbrus/besturingssystemen/blob/master/assignment-2/task-3/task.c#L104-149) in task.c which can be run by uncommenting the main function in task.c.

## How does it work?

We found this task to be fundamentally different from task 1 and that's why we have started almost completely from scratch.

The program uses a linked list structure to keep track of free slots. This is achieved by storing an integer in free
slots that points to the next free slot. Also, we need to keep track of the beginning (`free_slots.first`) of the
linked list, and the end (`free_slots.last`). We set up a struct (`linked_list`) to keep track of this. The first free
slot is the slot that will be allocated by `task_alloc`.

## Initialization
On the first run of `task_alloc` a block of memory is requested using `_sbrk`. The start of this block will be stored in
`memory_block_start`. This comes in handy later on in `task_free` to check whether the pointer is valid. The pointer
to the start the start of the memory block will be returned as result of this first `task_alloc` call.

### Allocating tasks
After the first call, each subsequent call will perform a few steps to find out which slot should be returned and
whether the memory block should be increased. First we're checking whether the linked list is empty. If it is not empty,
we differentiate between two cases. In either case `free_slots.first` is returned as result.

The first case is that the free slot contains a pointer to the next free slot. We then point `free_slots.first` to
this next slot. The second case is that this slot does not contain a pointer to a next free slot, which means this
is the last slot in the linked list. To denote we used the last slot and the linked list is now empty, we set
`free_slots.first` to `NULL`.

It may also happen that `task_alloc` is called when the linked list is empty. In this case we should allocate a
slot right after the the last allocated slot in this memory block. We keep track of this slot using the
`last_allocated_slot` variable. If this new slot is beyond the end of the memory block, then more memory is requested,
again using `_sbrk`.

### Freeing tasks
First a few checks are performed on the pointer. The pointer should be in the memory block range and it should not be
`NULL`. Then the slot to which the pointer points is appended at the and of the linked list. The current last free slot
is updated to point to this new last slot. To denote this is now the last slot we make sure to empty it.
