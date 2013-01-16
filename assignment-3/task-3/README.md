# Documentation on task 3
-------------------------

## How to run

First change the `EXAMPLE_FILE` constant defined in `rssfs.c` to the location of the bencoded
file to read from.

Then add the following to your /etc/system.conf:

    service rssfs {
    };

Finally, create the directory `/mnt/rssfs`. You should now be able to run `./mount` and access
the virtual file system.

## How does it work?

This solution is largely derived from task 1 and 2. First the bencoded file is parsed into a struct, then the virtual file system is set up. On initialize, each parsed item from the bencoded file is displayed as an individual file. Its filename is dervived from the guid. The filename cannot be longer than 24 characters.

The read hook is overwritten to show different contents depending on the callback data. We have
decided to go with json files, so we output the article as json.