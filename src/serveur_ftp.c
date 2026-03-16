#include "csapp.h"
#include "request.h"

#define PORT 2121
#define MAX_NAME_LEN 256
#ifndef POOL_SIZE
#define POOL_SIZE 20
#endif

int i;


void handler_chld(int signal) 
{
    wait(NULL);
    i--;
}

void handler_int(int signal)
{
    Kill(0, SIGKILL);
    while(waitpid(0, NULL, WNOHANG));
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

    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(PORT);
    for(i = 0; i < POOL_SIZE; i++)
    {
        if(Fork() == 0) {
            while (1) {
                connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                Getnameinfo((SA *) &clientaddr, clientlen,
                            client_hostname, MAX_NAME_LEN, 0, 0, 0);
                
                Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                        INET_ADDRSTRLEN);
                
                printf("server connected to %s (%s)\n", client_hostname,
                    client_ip_string);
                
                request_t request;
                read_request(&request, connfd);
                printf("%ld bytes reçu\n", strlen(request.path));

                Close(connfd);
            }
        }
    }
    while(1);
}

