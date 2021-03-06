# Documentation on task 3
-------------------------
## Preparation
To make sure all security measurements are turned off, run the following:

    $ sudo -i
    # echo 0 > /proc/sys/kernel/randomize_va_space

## How do we run it?

First, compile `secret_string.c`:

	$ gcc -o secret_string secret_string.c

Then, use the output of of this program as input to `as4_t3`.

	$ ./as4_t3 `./secret_string`

`secret_string` now generates a crafted string to buffer overflow `as4_t3`.

## How does it work?

We have discovered that the first 108 chars (90 for `buf`, then 18 for some stack management) of the string are used to get to the place in the stack where `i` is. The last characters are used to overwrite `i`. These characters are generated by setting an int to `0xdeadbeef` and then pointing a `*char` to it.