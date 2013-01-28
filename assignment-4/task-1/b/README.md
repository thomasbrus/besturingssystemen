# Documentation on task 1a
-------------------------
## How to run the program?  
Run

    $ sudo ./lazy --help 
to see all options.

## How does it work?
The program starts by checking whether it is run as root, and (when started with `--init`) if the assignments directory already exists and a group `teacher` already exists. If the directory or group already exists, the user is asked how he wants to handle those issues.

It then asks for the teacher account and an assignments directory is built in the current directory. When the script is run again with `--add-students`, for every given student account a directory within the assignments directory is created. Existence of every user account is checked before continuing the bash script.

### Rights management
Every subdirectory is owned by a student. A student cannot read, write or search/execute the contents of other folders at any time. If the the folders are locked down, students do not have any rights on their own directory. If the directories are unlocked, students can read write en execute in their directory. The teacher has always read, write and execute rights on the students directories (this is not possible without acl without using groups).

## What shortcomings do we still have?
* We also have to change rights of all folders to unlock or lock down the directory. This is not very effective since in fact all rights are equal. Moreover, if the list would become even larger this can be a pretty slow operation. This is still the case when using acl.