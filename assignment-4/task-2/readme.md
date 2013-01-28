# Documentation on task 2
-------------------------
## Fragment 1
### Vulnerabilities
The program puts a given input parameter in an array of chars. The original code did not check whether the given input would fit in the allocated char array. If the input was bigger than the initialized 90 chars, the data could have been written into the memory on a location allocated by another program. This would corrupt the program. Even worse, it could be exploited to execute malicious code. For an example of how this could be exploited, see task 4. 

### How we fixed it
We added the check `strlen(av[1]) <= 90` to make sure the input would fit in our memory block. 

## Fragment 2
### Vulnerabilities
This program has two vulnerabilities. First of all, the original program did not check whether there was a parameter set. Secondly, the program calls printf without a formatting parameter. This results in an uncontrolled format string: the original code interprets its argument as a format string. So by typing

    $ ./a.out %p

The original program does not output `%p`, but instead prints a pointer location. Thus, the original code could be used to print information other than the given input string. 

### How we fixed it
We added the format `%s` to printf() to ensure that the given input is printed as a string.

## Fragment 3
In the original code, it only checked whether the given memory block size was not too big. However, it was still allowed to call malloc(0) or call malloc() with a negative integer. In addition, if the function atoi() can not convert the given string to an integer (like "abc"), it returns 0, which is not useful in here. Therefore, the program now makes sure that the given integer is the range 0 and 101. Secondly, the program did not check whether a parameter (which is used for atoi() ) was given and whether the given parameter was not too long; a numeric overflow could have been the result.