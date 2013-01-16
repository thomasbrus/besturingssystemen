#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

#include <sys/stat.h>

#include <minix/drivers.h>
#include <minix/vtreefs.h>

#include "bencode.h"

// Common elements of an <ITEM> tag.
typedef struct rss_entry {
  char *title;
  char *description;
  char *link;
  char *guid;
  char *pubDate;
} rss_entry;

// Common elements of a <CHANNEL> tag.
typedef struct rss_body {
  char *title;
  char *description;
  char *link;
  char *language;
  struct rss_entry **item; 
} rss_body;

rss_body *my_rss_body;
struct inode_stat root_stat;

struct inode *get_inode_by_date (char *pubDate) {
  struct inode *result;
  struct inode *temp;
  struct inode_stat dir_stat;
  char month[4];
  char year[5];

  dir_stat.mode = S_IFDIR;
  dir_stat.uid = 0;
  dir_stat.gid = 0;
  dir_stat.size = 0;
  dir_stat.dev = NO_DEV;

  if (strcmp (pubDate, "") == 0){
    result = get_inode_by_name(get_root_inode(), "no_pubDate");
    if (result == 0){
      result = add_inode(get_root_inode(), "no_pubDate", NO_INDEX, &dir_stat, 0, 0);
    }
  } else {

    strncpy(month, pubDate+8, 3);
    month[3] = 0;
    strncpy(year, pubDate+12, 4);
    year[4] = 0;
    
  /*  printf("Month: %s\n", month);
    printf("Year: %s\n", year);*/

    temp = get_inode_by_name(get_root_inode(), year);

    if (temp == 0){
      temp = add_inode(get_root_inode(), year, NO_INDEX, &dir_stat, 0, 0);
      result = add_inode(temp, month, NO_INDEX, &dir_stat, 0, 0);
    } else {
      result = get_inode_by_name(temp, month);
      if (result == 0){
        result = add_inode(temp, month, NO_INDEX, &dir_stat, 0, 0);
      }
    }
  }

  return result;
}

static void initialize_hook(void) {
  int i = 0;
  int no_guid = 0;
  struct inode_stat file_stat;

  file_stat.mode = S_IFREG | 0444;
  file_stat.uid = 0;
  file_stat.gid = 0;
  file_stat.size = 0;
  file_stat.dev = NO_DEV;

  for (i = 0; my_rss_body->item[i]; i++) {
    char *title_with_slash;
    char filename[16];
    if (strcmp(my_rss_body->item[i]->guid, "") == 0){
      char index[5];
      sprintf (index, "%d", no_guid);
      strcpy(filename, "no_guid_");
      strcat(filename, index);
      strcat(filename, ".json");
      no_guid ++;
    } else {
      title_with_slash = strrchr(my_rss_body->item[i]->guid, '/');

      strncpy(filename, title_with_slash + 1, 10);
      filename[10] = 0;
      strcat (filename, ".json");
    }

    add_inode(get_inode_by_date(my_rss_body->item[i]->pubDate), filename, NO_INDEX, &file_stat, 0, (cbdata_t) i);
  }
}

static int read_hook(struct inode *inode, off_t offset, char **ptr, size_t *len, cbdata_t cbdata) {
  /* This hook will be called every time a regular file is read. We use
   * it to dynamically generate the contents of our file.
   */
  static char data[1023];
  int i = (int) cbdata;

  /* Generate the contents of the file into the 'data' buffer.
   */

  sprintf(data,
    "{\n"
    "\t\"title\": \"%s\",\n"
    "\t\"description\": \"%s\",\n"
    "\t\"link\": \"%s\",\n"
    "\t\"guid\": \"%s\",\n"
    "\t\"pubDate\": \"%s\"\n"
    "}\n",
    my_rss_body->item[i]->title,
    my_rss_body->item[i]->description,
    my_rss_body->item[i]->link,
    my_rss_body->item[i]->guid,
    my_rss_body->item[i]->pubDate
  );

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

  if (*len > strlen(data) - offset) *len = strlen(data) - offset;

  return OK;
}


// Reads a file into `contents` and returns the length
// Source: http://stackoverflow.com/questions/13223666/c-fread-not-getting-entire-file
unsigned int read_file(const char *filename, char **contents) {
  struct stat stbuf;
  unsigned int length = 0;

  // Open a file descriptor
  int fd = open(filename, O_RDONLY);

  // Check that file descriptor was opened
  if (fd == -1) {
    fprintf(stderr, "Fatal Error: Failed to open the file for fstat (file: %s)!\r\n", filename);
    exit(-1);
  }

  // Get information from fstat
  if (fstat(fd, &stbuf) == -1) {
    fprintf(stderr, "Fatal Error: Failed to find file length (file: %s)!\r\n", filename);
    exit(-1);
  }

  // Store the file length
  length = stbuf.st_size;

  // Close file descriptor
  if (close(fd) == -1) {
    fprintf(stderr, "Fatal Error: Failed to close file descriptor (file: %s)!\r\n", filename);
    exit(-1);
  }

  // Check if the file contains data
  if (length > 0) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");

    // Check that the file was opened
    if (file != NULL) {
      freopen(filename, "r", file);
      // Prepare the contents variable
      *contents = 0;
      *contents = (char*)calloc(length + 1, sizeof(char));

      // Read the file and put it in the contents variable
      int length_read = fread(*contents, length, 1, file);

      // Check for file read error
      if (ferror(file)) {
        fprintf(stderr, "Fatal Error: Failed to read file (file: %s)!\r\n", filename);
        exit(-1);

      } else if (length_read != 1 || strlen(*contents) < length) {
        fprintf(stderr, "Fatal Error: File read error (file: %s)!\r\n", filename);
        fprintf(stderr, "Characters expected: %d, Characters read: %d\r\n", length, strlen(*contents));
        fprintf(stderr, "Content read:\r\n%s\r\n", *contents);
        fprintf(stderr, "Aborting! %s\r\n", filename);

        // Close file and exit
        fclose(file);
        exit(-1);
      }

      // Close binary file
      fclose(file);

    } else {
      fprintf(stderr, "Fatal Error: Failed to open the file: %s!\r\n", filename);
      exit(-1);
    }

  } else {
    fprintf(stderr, "Fatal Error: File was empty (file: %s)!\r\n", filename);
    exit(-1);
  }

  return length;
}

rss_entry *handle_item(be_node *node){
  // Allocate memory for result
  rss_entry *result =  malloc(sizeof(rss_entry));

  result->title = "";
  result->description = "";
  result->link = "";
  result->guid = "";
  result->pubDate = "";

  if (node->type == BE_DICT) {
    size_t i;

    // This node is a dictionary, as expected
    // Now read through all elements and put the common ones in our struct
    for (i = 0; node->val.d[i].val; ++i) {
      if (strcmp(node->val.d[i].key, "title") == 0) {
        result->title = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "description") == 0) {
        result->description = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "link") == 0) {
        result->link = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "guid") == 0) {
        result->guid = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "pubDate") == 0) {
        result->pubDate = node->val.d[i].val->val.s;
      }
    }
  }

  return result;
} 

rss_body *handle_body(be_node *node) {
  // Allocate memory for result
  rss_body *result = (rss_body *) malloc(sizeof(rss_body));

  if (node->type == BE_DICT) {
    size_t i, j;

    // This node is a dictionary, as expected
    // Now read through all elements and put the common ones in our struct
    for (i = 0, j = 0; node->val.d[i].val; ++i) {     
      if (strcmp(node->val.d[i].key, "title") == 0) { 
        result->title = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "description") == 0) {
        result->description = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "link") == 0) {
        result->link = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "language") == 0) {
        result->language = node->val.d[i].val->val.s;

      } else if (strcmp(node->val.d[i].key, "item:") == 1) {
        // TODO: improve this
        // Initial memory to be used for 'item'-entries
        int toInit = 5;

        if (j == 0) {
          // First run, so allocate the memory for the 'toInit' elements
          result->item = malloc(sizeof(rss_entry *) * toInit);

        } else if (j >= toInit) {
          // There are more elements then we expected in 'toInit', so allocate extra memory
          result->item = realloc(result->item,(j+1)*sizeof(rss_entry));         
        }
        // Add the 'item'-entry and increase counter 'j'
        result->item[j++] = handle_item(node->val.d[i].val);                           
      }
    }
  } 

  return result;
}

void setup_root_dir(void) {
  /* Fill in the details to be used for the root inode. It will be a
   * directory, readable and searchable by anyone, and owned by root.
   */
  root_stat.mode = S_IFDIR | 0555;
  root_stat.uid = 0;
  root_stat.gid = 0;
  root_stat.size = 0;
  root_stat.dev = NO_DEV; 
}

/* The table with callback hooks. */
struct fs_hooks hooks = {
  initialize_hook,
  NULL, /* cleanup_hook */
  NULL, /* lookup_hook */
  NULL, /* getdents_hook */
  read_hook,
  NULL, /* rdlink_hook */
  NULL  /* message_hook */
};

rss_body *parse_bencode_from_file(const char *filename) {
  be_node *node;
  char *contents = 0;
  unsigned int length = read_file(filename, &contents);

  if ((node = be_decoden(contents, length))) {
    return handle_body(node);

  } else {
    return NULL;
  }
}

int main(int argc, char *argv[]) {

  /* Dit kan per systeem verschillen! */
  my_rss_body = parse_bencode_from_file("/mnt/hgfs/GitHub/besturingssystemen/assignment-3/task-4/file.ben");
  setup_root_dir();
  start_vtreefs(&hooks, 10, &root_stat, 0);

  return 0;

}
