#include <stdio.h>
#include <string.h>

void youWin() {
    printf( "Yay, you did it!\n" );
}

int main( int ac, char** av )
{
    int i = 0;
    char buf[90];
    if( ac > 1 )
        strcpy( buf, av[1] );

    if( i == 0xdeadbeef )
        youWin();

    printf( "Value of i: 0x%0x\n", i );
    return 0;
}

