# Documentation on task 1a
--------------------------
## How to run the program?  
Run

    $ sudo ./lazy --help 
to see all options.

## How does it work?
The program starts by checking whether it is run as root, and (when started with `--init`) if the assignments directory already exists and a group `teacher` already exists. If the directory or group already exists, the user is asked how he wants to handle those issues.

It then asks for the teacher account and an assignments directory is built in the current directory. When the script is run again with `--add-students`, for every given student account a directory within the assignments directory is created. Existence of every user account is checked before continuing the bash script.

### Rights management
The assignments directory is owned by the `teacher` group, and every student directory is owned by the student himself. Because the `setgid` bit is set, all new subdirectories will inherit the group from their parent (thus, `teacher`). After set-up the new directories are locked down (or all with `--lock-down`) by putting 0070 rights on those student directories. In this manner, only the group has r+w+x rights, and because the teacher account is part of the group which owns the directories, he can access everything.

When is it unlocked (with `--unlock`), the rights on the student directories are set to 0770. By this, the owner (which is for every student directory the student himself) gets r+w+x rights. Everybody else (like other students) cannot see anything within the folder.
`sticky` and `setuid` bits are not used.

## What shortcomings do we still have?
* In this solution we've used a teacher group only to be able to set a distinct right for one particular user. This is somehow a misuse of the system, since group implies that more than one user are part of it. Also, if the `teacher` group already exists, we would have to change the whole program to workaround that issue.

* We also have to change rights of all folders to unlock or lock down the directory. This is not very effective since in fact all rights are equal. Moreover, if the list would become even larger this can be a pretty slow operation.

* One requirement that was impossible to fulfill was that student users couldn't see the existence of the directories for each other. With the execute right revoked on a directory it turned out to be possible to disable listing (with `ls`) on the assignments directory, but it was then impossible to open a child directory within.