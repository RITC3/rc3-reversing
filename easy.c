#include <stdio.h>

int main(int argc, char *argv[])
{
    char one = getchar();
    char two = getchar();
    char three = getchar();

    if (one == two && three == '#'){
        puts("\nYou win! The flag is RC3-ICAN-1337");
    }
    else{
        puts("\nbetter luck next time");
    }

    return 0;
}
