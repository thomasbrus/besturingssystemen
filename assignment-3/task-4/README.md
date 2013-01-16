# Documentation on task 4
-------------------------

## How to run

Follow the instructions from task 3

## The directory structure imposed on the file system

There are two categories of of directories imposed on the file system, depending on whether or not the RSS entry contains a pubDate attribute:

### The RSS entry contains a pubDate attribute

In this case, the following directory structure is used:

/<mount location>/<yyyy>/<month>/<file name>.json

Where month is a three letter abbreviation of the month, beginning with a capital.

### The RSS entry does not contain a pubDate attribute

In this case, the following directory structure is used:

/<mount location>/no_pubDate/<file name>.json

The file name is the guid of the RSS entry if it exists. If it does not, "no_guid_<index>" is used as the file name, where <index> represents an integer. This integer is different by every RSS entry that does not have a guid in order to avoid file conflicts.