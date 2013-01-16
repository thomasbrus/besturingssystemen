# Documentation on task 2
-------------------------
# How to run the program?

To compile the program, use the following command:
$ gcc bencode.c benc2json.c -Wall

To run the program, use the following command:
$ ./a.out <inputfile>

The input file is supposed to be a bencoded rss file, like file.ben is. For example:
$ ./a.out file.ben

# How does the program work?
## Starting the program
The program starts with checking whether a parameter was given. Afer that, it initializes some variables (necessairy to store the output strings in) and starts the reading procedure.

## Reading the file
First of all, the program tries to get the size of the file. It uses stat() to gather information about the file, including its size. After that, the program tries to open the file and get read acces to it, using fopen(). The result is put in a FILE*, which then holds information about the stream (like the buffer pointer). If it successfully gained read access, the program uses malloc() to allocate a memory block as large as the file itself. If the allocation was successful, the program uses fread() to write from the stream to the allocated memory. Finally, it closes the file associated with the stream using fclose() and returns the pointer to the allocated memory (which holds the file contents).

## Parsing with bencode.c
The program uses bencode.c to parse the file contents in bencode structs (which are declared in bencode.h), according to the bencode standard. If bencode.c was able to decode the file without errors, the program continues.  

## Filling the structs
Now the program tries to put the retrieved bencode structure in an RSS structure. It checks whether common elements in the <CHANNEL> tags are available; if they are, they are put in an rss_channel struct (which is declared in benc2json.c:17). When an <ITEM> tag is detected, a similar process takes place: the common elements in the <ITEM> tag are put in their own rss_entry struct (declared in benc2json.c:8). The struct is added to the rss_channel struct. Since there are is a variable ammount of <ITEM> elements in a <CHANNEL>, the memory for the various rss_entry structs is dynamically allocated. After all bencode elements are checked, the program continues. 

## Outputting the JSON
Now the program reads out the created rss_channel struct (including the rss_entry structs) and outputs it in the JSON standard.
