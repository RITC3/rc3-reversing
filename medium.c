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
pthread_mutex_t tmutex;

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
    if ( pthread_mutex_init(&tmutex, NULL) != 0 ){
        perror("mutex init failed");
        close(lsock);
        return 1;
    }

    while (1){
        rsock = accept(lsock, (sockaddr *)&rsin, &rsin_len);
        pthread_mutex_lock(&tmutex);
        pthread_create(&pid, NULL, handler, (void *)&rsock);
        pthread_mutex_unlock(&tmutex);
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
    pthread_mutex_lock(&tmutex);
    if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
        perror("send");
        close(*rsock);
        pthread_mutex_unlock(&tmutex);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&tmutex);

    unsigned char key[17];
    key[0] = 72;
    key[1] = 65;
    key[2] = 67;
    key[3] = 75;
    key[4] = 32;
    key[5] = 84;
    key[6] = 72;
    key[7] = 69;
    key[8] = 32;
    key[9] = 71;
    key[10] = 73;
    key[11] = 66;
    key[12] = 83;
    key[13] = 79;
    key[14] = 78;
    key[15] = 11;

    if (recv(*rsock, rBuf, BUFSIZE, 0) == -1){
        perror("recv");
        close(*rsock);
        pthread_exit(NULL);
    }

    key[sizeof(key)-1] = '\0';

    pthread_mutex_lock(&tmutex);
    if (strstr(rBuf, (char *)key) != NULL){
        if ((file = fopen("flag2.txt", "r")) != NULL){
            fgets(fBuf, BUFSIZE, file);
            fclose(file);
        } else {
            printf("Error reading from file");
        }
        strncpy(sBuf, "\nYou win! The flag is ", BUFSIZE-1);
        strncat(sBuf, fBuf, BUFSIZE);
        if (send(*rsock, sBuf, BUFSIZE, 0) == -1){
            perror("send");
            close(*rsock);
            pthread_mutex_unlock(&tmutex);
            pthread_exit(NULL);
        }
    } else {
        strncpy(sBuf, "Better luck next time, kid\n", BUFSIZE);
        if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
            perror("send");
            close(*rsock);
            pthread_mutex_unlock(&tmutex);
            pthread_exit(NULL);
        }
    }

    pthread_mutex_unlock(&tmutex);
    close(*rsock);
    pthread_exit(NULL);
}
