#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 13

static void uncrypt(char *c);
int main(int argc, char *argv[])
{
    srand(time(NULL));
    int num = rand() + rand() - rand();
    int guess;
    printf("I'm thinking of a number... Take a guess: ");
    scanf("%d", &guess);
    /*xor by 69*/
    unsigned char arr[] = {
     0x17,
     0x6,
     0x76,
     0x68,
     0xD,
     0x4,
     0x1D,
     0x1,
     0x68,
     0x73,
     0x73,
     0x73,
     0x73,
     0x00
    };

    if (guess == num){
        uncrypt(arr);
        printf("\nYou win! The flag is %s\n", arr);
    } else {
        printf("\nYou win! The flag is %s\n", arr);
    }
    return 0;
}

static void uncrypt(char *c){
    for (int i=0;i<SIZE;++i){
        c[i] ^= 69;
    }
}
