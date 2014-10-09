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
typedef int sock;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
#define BUFSIZE 1028
#define PORT 54321
#define MAX_CONNECTIONS 30

void *handler(void * pSock);

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
    memset(rBuf, '\0', BUFSIZE);
    memset(sBuf, '\0', BUFSIZE);
    memset(fBuf, '\0', BUFSIZE);
    strncpy(sBuf, "Enter the super secret key: ", BUFSIZE);
    if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
        perror("send");
        close(*rsock);
        pthread_exit(NULL);
    }
    unsigned char key[19];
    key[0] = 72;
    key[1] = 65;
    key[2] = 67;
    key[4] = 75;
    key[5] = 32;
    key[6] = 84;
    key[7] = 72;
    key[8] = 69;
    key[9] = 32;
    key[10] = 71;
    key[11] = 73;
    key[12] = 66;
    key[13] = 83;
    key[14] = 79;
    key[15] = 78;
    key[16] = 11;

    if (recv(*rsock, rBuf, BUFSIZE, 0) == -1){
        perror("recv");
        close(*rsock);
        pthread_exit(NULL);
    }
    key[sizeof(key)-1] = '\0';
    if ((file = fopen("flag2.txt", "r")) != NULL){
        fgets(fBuf, BUFSIZE, file);
        fclose(file);
    } else {
        printf("Error reading from file");
    }

    if (strstr(rBuf, (char *)key) != NULL){
        strncpy(sBuf, "\nYou win! The flag is ", BUFSIZE-1);
        strncat(sBuf, fBuf, BUFSIZE);
        if (send(*rsock, sBuf, BUFSIZE, 0) == -1){
            perror("send");
            close(*rsock);
            pthread_exit(NULL);
        }
    } else {
        strncpy(sBuf, "Better luck next time, kid\n", BUFSIZE);
        if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
            perror("send");
            close(*rsock);
            pthread_exit(NULL);
        }
    }
    close(*rsock);
    pthread_exit(NULL);
}
