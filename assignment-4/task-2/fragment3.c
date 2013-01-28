#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int ac, char** av )
{
	if (ac == 2 && strlen(av[1]) < 4){ /*check whether parameter is given; check parameter is not too long*/
	    int numBytes = atoi( av[1] );
	    if (numBytes > 100 || numBytes <= 0) { /*check whether numBytes is not too big, and not invalid*/
	        return -1;
	    }    
	    int* something = (int*)malloc( numBytes );
	} else {
    return -1;
  }
}