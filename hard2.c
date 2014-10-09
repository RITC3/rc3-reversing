#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

typedef int sock;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#define BUFSIZE 1028
#define PORT 12345
#define MAX_CONNECTIONS 30
#define SIZE 13

void *handler(void * pSock);
static void uncrypt(char *c);

int main()
{
    sock rsock, lsock;
    sockaddr_in lsin, rsin;
    pthread_t pid;
    memset(&lsin, 0, sizeof(sockaddr_in));
    memset(&rsin, 0, sizeof(sockaddr_in));

    lsin.sin_family = AF_INET;
    lsin.sin_port = htons(PORT);
    lsin.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((lsock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return 1;
    }
    if (bind(lsock, (sockaddr *)&lsin, sizeof(lsin)) == -1){
        perror("bind");
        return 1;
    }
    if (listen(lsock, MAX_CONNECTIONS) == -1){
        perror("listen");
        return 1;
    }

    socklen_t rsin_len = sizeof(rsin);

    while (1){
        rsock = accept(lsock, (sockaddr *)&rsin, &rsin_len);
        pthread_create(&pid, NULL, handler, (void *)&rsock);
    }

    return 0;
}


void *handler(void *pSock){
    sock *rsock = (sock *)pSock;
    FILE *file;
    char sBuf[BUFSIZE];
    char rBuf[BUFSIZE];
    char fBuf[BUFSIZE];
    srand(time(NULL));
    int num = rand() + rand() - rand();
    int guess;
    memset(rBuf, '\0', BUFSIZE);
    memset(sBuf, '\0', BUFSIZE);
    memset(fBuf, '\0', BUFSIZE);
    strncpy(sBuf, "I'm thinking of a number... Take a guess: ", BUFSIZE);
    if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
        perror("send");
        close(*rsock);
        pthread_exit(NULL);
    }
    /*xor by 69*/
    unsigned char key[] = {
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

    if (recv(*rsock, rBuf, BUFSIZE, 0) == -1){
        perror("recv");
        close(*rsock);
        pthread_exit(NULL);
    }
    guess = atoi(rBuf);

    if ((file = fopen("flag2.txt", "r")) != NULL){
        fgets(fBuf, BUFSIZE, file);
        fclose(file);
    } else {
        printf("Error reading from file");
    }

    if ( guess == num){
        uncrypt((char *)key);
        strncpy(sBuf, "\nYou win! The flag is ", BUFSIZE-1);
        strncat(sBuf, fBuf, BUFSIZE);
        if (send(*rsock, sBuf, BUFSIZE, 0) == -1){
            perror("send");
            close(*rsock);
            pthread_exit(NULL);
        }
    } else {
        strncpy(sBuf, "NAH\n", BUFSIZE);
        if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
            perror("send");
            close(*rsock);
            pthread_exit(NULL);
        }
    }
    close(*rsock);
    pthread_exit(NULL);
}


static void uncrypt(char *c){
    for (int i=0;i<SIZE;++i){
        c[i] ^= 69;
    }
}
