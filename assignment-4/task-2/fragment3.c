#include <stdio.h>
#include <stdlib.h>

int main( int ac, char** av )
{
    int numBytes = atoi( av[1] );
    if( numBytes > 100 || numBytes <= 0) { /*check whether numBytes is not too big, and not invalid*/
        return -1;
    }    
    int* something = (int*)malloc( numBytes );
}