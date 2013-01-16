#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "bencode.h"

//Common elements of an <ITEM> tag.
typedef struct rss_entry {
	char *title;
	char *description;
	char *link;
	char *guid;
	char *pubDate;
} rss_entry;

//Common elements of a <CHANNEL> tag.
typedef struct rss_channel {
	char *title;
	char *description;
	char *link;
	char *language;
	struct rss_entry **item; 
} rss_channel;


char *read_file(const char *file, long long *len) {
	//Reads a file and returns the contents as a string.
	//Original source: test.c (provided by wiki)
	struct stat st;
	char *ret = NULL;
	FILE *fp;
	//Get the file statistics
	if (stat(file, &st)) 
		return ret;
	//Get the file size
	*len = st.st_size;
	//Create a FILE stream
	fp = fopen(file, "r");
	if (!fp)
		return ret;
	//Allocate the memory necessary (= file size)
	ret = malloc(*len);
	if (!ret)
		return NULL;
	//Start reading, put output in memory
	fread(ret, 1, *len, fp); 
	//Close stream
	fclose(fp);
	//Return pointer to file contents
	return ret;
}

rss_entry *handleItem(be_node *node) {
	size_t i;
	//Allocate memory for result
	rss_entry *re =  malloc(sizeof(rss_entry));
	if(node->type == BE_DICT) {
		//This node is a dictionary, as expected
		//Now read through all elements and put the common ones in our struct
		for (i = 0; node->val.d[i].val; ++i) {
			if(strcmp(node->val.d[i].key, "title") == 0){
				re->title = node->val.d[i].val->val.s;
			} else 				
			if(strcmp(node->val.d[i].key, "description") == 0){
				re->description = node->val.d[i].val->val.s;
			} else 				
			if(strcmp(node->val.d[i].key, "link") == 0){
				re->link = node->val.d[i].val->val.s;
			} else 				
			if(strcmp(node->val.d[i].key, "guid") == 0){
				re->guid = node->val.d[i].val->val.s;
			} else 				
			if(strcmp(node->val.d[i].key, "pubDate") == 0){
				re->pubDate = node->val.d[i].val->val.s;
			}			
		}
	}
	return re;		
} 

rss_channel *handleChannel(be_node *node) {
	size_t i, j;
	//Allocate memory for result
	rss_channel *result = (rss_channel *) malloc(sizeof(rss_channel));
	if (node->type == BE_DICT) {
		//This node is a dictionary, as expected
		//Now read through all elements and put the common ones in our struct
		for (i = 0, j = 0; node->val.d[i].val; ++i) {			
			if (strcmp(node->val.d[i].key, "title") == 0) {	
				result->title = node->val.d[i].val->val.s;
			} else 
			if (strcmp(node->val.d[i].key, "description") == 0) {	
				result->description = node->val.d[i].val->val.s;
			} else 
			if (strcmp(node->val.d[i].key, "link") == 0) {	
				result->link = node->val.d[i].val->val.s;
			} else 
			if (strcmp(node->val.d[i].key, "language") == 0) {	
				result->language = node->val.d[i].val->val.s;
			} else	
			if (strncmp(node->val.d[i].key, "item:", 5) == 0) {
				int toInit = 5;	//Initial memory to be used for 'item'-entries				
				if (j == 0) {
					//First run, so allocate the memory for the 'toInit' elements
					result->item =  malloc(toInit * sizeof(rss_entry *));												
				} else
				if(j >= toInit){
					//There are more elements then we expected in 'toInit', so allocate extra memory
					result->item =  realloc(result->item, (j + 1) * sizeof(rss_entry *));					
				}
				//Add the 'item'-entry and increase counter 'j'
				result->item[j++] =  handleItem(node->val.d[i].val);													 	
			}
		}
	}	
	return result;
}

void parseToJSON(rss_channel *rssBody) {
	//Print the rss_channel struct in JSON format.
	printf("{");
	printf("\"title\" : \"%s\",",rssBody->title);
	printf("\"description\" : \"%s\",",rssBody->description);
	printf("\"language\" : \"%s\",",rssBody->language);
	printf("\"link\" : \"%s\",",rssBody->link);;
	printf("\"items\" : ");
	int i;
	for (i = 0; rssBody->item[i]; i++) {
		printf("{");
		printf("\"title\" : \"%s\",",rssBody->item[i]->title);
		printf("\"description\" : \"%s\",",rssBody->item[i]->description);
		printf("\"guid\" : \"%s\",",rssBody->item[i]->guid);
		printf("\"link\" : \"%s\",",rssBody->item[i]->link);
		printf("\"pubDate\" : \"%s\"",rssBody->item[i]->pubDate);
		printf("}");
		if(rssBody->item[i+1]) { printf(", "); }
	}
	printf("}\n");
}

int main(int argc, char *argv[])
{
	if(argc == 2) {
		char *buf;
		long long len;
		be_node *n;

		buf = read_file(argv[1], &len);
		if (!buf) {
			printf("Could not open file.\n");
		}

		//Use bencode decoder to parse the file into the bencode structure
		n = be_decoden(buf, len); 
		if (n) {	
			//Parsing succesful; now parse the output to our RSS structs
			rss_channel *rssPage = handleChannel(n);
			//Give some output
			parseToJSON(rssPage); 
			//Free used memory
			free(rssPage);			
			be_free(n);
		} else {
			//Parsing unsuccessful
			printf("Parsing failed!\n");
		}
		if (buf != argv[1])
			free(buf);
	} else {
		printf("Please provide one filename as a parameter.\n");
	}
	return 0;
}
