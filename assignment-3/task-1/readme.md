# Documentation on task 1
-------------------------

## How to run

Create a directory /mnt/1f and add to /etc/system.conf the following line:

    service 1f{};

Then thereafter, in a folder containing the source files:

    $ make
    # make install
    # mount -t 1f none /mnt/1f

In /mnt/1f the file system can be found.

## How does it work?

Our 'one file' file system is based on the VTreeFS. In this system there is one top node (root), and all directories and files are chained to it. In this little program one directory (as3) is chained to the root and a file (t1) is chained to the as3 dir. For all directories and files we put the minimal rights to view the nodes (555 for directories, 444 for files), so in fact the file system is read-only. When a file is read, the reader function is called. For this application it doesn't do much more than giving some static string, independently of the file requested. It returns an EOF when the end of the file is hit. Because of this way of dynamically building files, an `ls -l` command will show a size of 0 bytes for files on this file system.

Large parts of the source code (1f.c and Makefile) are derived from the VTreeFS example on http://wiki.minix3.org/en/DevelopersGuide/VTreeFS
