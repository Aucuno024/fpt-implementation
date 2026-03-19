#include "csapp.h"
#include "request.h"
#include "response.h"

#define PORT 2121
#define MAX_NAME_LEN 256
#ifndef NB_SLAVE
#define NB_SLAVE 2
#endif

#define SPEAKER "Raccoon"


void handler_chld(int signal) 
{
    wait(NULL);
}

void handler_int(int signal)
{
    #ifdef DEBUG
        Kill(0, SIGKILL);
        while(printf("%s say \" Catch%d\"\n",SPEAKER, waitpid(0, NULL, WNOHANG)));
    #else 
        Kill(0, SIGKILL);
        while(waitpid(0, NULL, WNOHANG));
    #endif
    exit(0);
}
/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supporteded)
 */
int main(int argc, char **argv)
{
    Signal(SIGCHLD, handler_chld);
    Signal(SIGINT, handler_int);
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    char slave_ip[NB_SLAVE][INET_ADDRSTRLEN];
    int i = 0, connected = 0;
    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(PORT);
    #ifdef DEBUG
        printf("%s say \"client len :%u listenfd : %d\"\n", SPEAKER, clientlen, listenfd);
    #endif
    while (1) {
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen,
                        client_hostname, MAX_NAME_LEN, 0, 0, 0);
            
        Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                INET_ADDRSTRLEN); 
        #ifdef DEBUG
            printf("%s say \"Server connected to %s (%s)\"\n", SPEAKER, client_hostname, client_ip_string);
        #endif

        request_t* request = malloc(sizeof(request_t));
        if (request == NULL) 
        {
            #ifdef DEBUG
                printf("%s say \"Request can't allocate\"\n", SPEAKER);
            #endif
            Close(connfd);
            continue;
        }
        if (read_request(request, connfd)) 
        {
        #ifdef DEBUG
            printf("%s say \"Failed to read request (size: %ld)\"\n", SPEAKER, sizeof(request_t));
        #endif
        free(request);
        send_error(connfd, ERROR_READ_REQUEST);
        break;
        }
        char content[MAXLINE];
        typereq_t typereq;
        decode_request(request, &typereq, content);
        free(request);
        if (typereq == ADD && connected + 1 < NB_SLAVE)
        {
            strcpy(slave_ip[connected++], content);
            send_response(connfd, "OK\n", BYE);
        } else if (typereq != ADD)
        {
            if(connected)
            {
                i = (i + 1) % connected;
                #ifdef DEBUG
                    printf("%s say \"Provide ip %s\"\n", SPEAKER, slave_ip[i]);
                #endif
                send_response(connfd, slave_ip[i], BYE);
            } else 
            {
                send_error(connfd, MAX_SLAVE_ERROR);
            }
        } else 
        {
            send_error(connfd, MAX_SLAVE_ERROR);
        }
        Close(connfd);
    }
}

