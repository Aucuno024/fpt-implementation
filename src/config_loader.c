#include "config_loader.h"
#include "stdio.h"
#include "csapp.h"

#define SPEAKER "grub"

int load_config_slave(config_slave_t **configptr , char filename[]) 
{ 
    config_slave_t *config = *configptr;
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
    
    memcpy(config->ip_master, buf, INET_ADDRSTRLEN);
    config->ip_master[INET_ADDRSTRLEN -1] = '\0';
    close(fd);

    return 0;
}

int load_config_master(config_master_t **configptr, char filename[], int max_slave)
{
    config_master_t *config = *configptr;
    int fd = Open(filename, O_RDONLY, 0);
    int n;

    if(fd == -1)
        return 1;

    config->number = 0;
    config->ip_slaves = malloc(sizeof(char *) * max_slave);
    char buf[MAXBUF];
    while((n = read(fd, buf, INET_ADDRSTRLEN)) && config->number < max_slave)
    {
        int i = 0;
        config->ip_slaves[config->number++] = malloc(sizeof(INET6_ADDRSTRLEN) + 1);
        for(; i < n; i++)
        {
            if(buf[i] == '\n' || buf[i] == '\0')
                break;

            config->ip_slaves[config->number - 1][i] = buf[i];
        }

        config->ip_slaves[config->number - 1][i] = '\0';
        #ifdef DEBUG
            printf("%s\n", config->ip_slaves[config->number - 1]);
        #endif
    }
    return 0;
}

int free_config_slave(config_slave_t *config)
{
    if(!config)
        return 0;

    free(config);

    return 1;
}

int free_config_master(config_master_t *config)
{
    if(!config)
        return 0;

    if(!config->ip_slaves)
    {
        free(config);

        return 2;
    }

    for(int i = 0; i < config->number; i++)
    {
        free(config->ip_slaves[i]);
    }

    free(config->ip_slaves);
    free(config);

    return 1;
}