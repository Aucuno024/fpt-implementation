#include "config_loader.h"
#include "stdio.h"
#include "csapp.h"

#define SPEAKER "grub"

int init_config_slave(config_slave_t *config, char filename[]) 
{ 
    int fd = Open(filename, O_RDONLY, 0);

    if(fd == -1) 
        return 1;
    
    char buf[INET_ADDRSTRLEN];
    int n = read(fd, buf, INET_ADDRSTRLEN - 1);

    #ifdef DEBUG
        printf("%s say \"buf :%s size : %d fd : %d\"\n", SPEAKER, buf, n, fd);
    #endif

    if(n < 7)
    {
        close(fd);
        return 1;
    }

    config = malloc(sizeof(config_slave_t));
    memcpy(config->ip_master, buf, INET_ADDRSTRLEN);
    config->ip_master[INET_ADDRSTRLEN -1] = '\0';
    close(fd);

    return 0;
}

int init_config_master(config_master_t *config, char filename[])
{
    int fd = Open(filename, O_RDONLY, 0);

    if(fd == -1)
        return - 1;

    
    
    return 0;
}