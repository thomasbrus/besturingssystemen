#include <string.h>

int main( int ac, char** av )
{
    char buf[96];
    if( ac > 1 )
        strcpy( buf, av[1] );

    return 0;
}

