#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <assert.h>

static void initialize(void)
{       
        /* This hook will be called once, after VTreeFS has initialized.
         */
        struct inode_stat dir_stat, file_stat;
        struct inode *dir, *file;

        /* Fill in the details to be used for the as3 inode. It will be a
         * directory, readable and searchable by anyone, and owned by root.
         */
        dir_stat.mode = S_IFDIR | 0555;
        dir_stat.uid = 0;
        dir_stat.gid = 0;
        dir_stat.size = 0;
        dir_stat.dev = NO_DEV;

        dir = add_inode(get_root_inode(), "as3", NO_INDEX, &dir_stat, 0, (cbdata_t) 2);

        /* We create one regular file in the as3 directory. The file is
         * readable by everyone, and owned by root. Its size as returned by for
         * example stat() will be zero, but that does not mean it is empty.
         * For files with dynamically generated content, the file size is
         * typically set to zero.
         */
        file_stat.mode = S_IFREG | 0444;
        file_stat.uid = 0;
        file_stat.gid = 0;
        file_stat.size = 0;
        file_stat.dev = NO_DEV;

        /* Now create the actual file. It is called "test" and does not have an
         * index number. Its callback data value is set to 1, allowing it to be
         * identified with this number later.
         */
        file = add_inode(dir, "t1", NO_INDEX, &file_stat, 0, (cbdata_t) 1);

        assert(file != NULL);
}

static int reader(struct inode *inode, off_t offset, char **ptr,
        size_t *len, cbdata_t cbdata)
{
        /* This hook will be called every time a regular file is read. We use
         * it to dynamically generate the contents of our file.
         */
        static char data[31];
        const char *str;

        /* We have only a single file. With more files, cbdata may help
         * distinguishing between them.
         */
        assert((int) cbdata == 1);

        /* Generate the contents of the file into the 'data' buffer.
         */
        str = "Hello world! Group 9 was here.\n";

        strcpy(data, str);

        /* If the offset is beyond the end of the string, return EOF. */
        if (offset > strlen(data)) {
                *len = 0;

                return OK;
        }

        /* Otherwise, return a pointer into 'data'. If necessary, bound the
         * returned length to the length of the rest of the string. Note that
         * 'data' has to be static, because it will be used after this function
         * returns.
         */
        *ptr = data + offset;

        if (*len > strlen(data) - offset)
                *len = strlen(data) - offset;

        return OK;
}

/* The table with callback hooks. */
struct fs_hooks hooks = {
        initialize,
        NULL, /* cleanup_hook */
        NULL, /* lookup_hook */
        NULL, /* getdents_hook */
        reader,
        NULL, /* rdlink_hook */
        NULL  /* message_hook */
};

int main(void)
{
        struct inode_stat root_stat;

        /* Fill in the details to be used for the root inode. It will be a
         * directory, readable and searchable by anyone, and owned by root.
         */
        root_stat.mode = S_IFDIR | 0555;
        root_stat.uid = 0;
        root_stat.gid = 0;
        root_stat.size = 0;
        root_stat.dev = NO_DEV;

        /* Now start VTreeFS. Preallocate 3 inodes.
         */
        start_vtreefs(&hooks, 3, &root_stat, 0);

        /* The call above never returns. This just keeps the compiler happy. */
        return 0;
}
