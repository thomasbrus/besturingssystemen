#include <stdio.h>
#include <string.h>

int main( int ac, char** av )
{
    char buf[10];
    if( ac > 1 && strlen(av[1]) <= 90){ /* check whether string is not too long */
        strcpy( buf, av[1] );        
    }
}
