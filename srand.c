#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    srand(atoi(argv[1]));
    for (int i=0; i<10; ++i){
        printf("%10d %10d %10d %10d %10d\n",rand(), rand(), rand(), rand(), rand());
    }
    return 0;
}
