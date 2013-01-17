#include <stdio.h>
#include <stdlib.h>

int main( int ac, char** av )
{
	if(ac == 2){	/* check whether exactly one parameter is set */
    	printf("%s", av[1] ); /* safely print input as string */
	}
}