#include "config_loader.h"


int init_config_slave(config_slave_t *config, char *filename) 
{
    int fd = open(filename, O_WRONLY);

    if(fd == -1) 
        return 1;
    
    char buf[SIZE_IP];
    int n = read(fd, buf, SIZE_IP - 1);

    if(n != SIZE_IP -1)
    {
        close(fd);
        return 1;
    }
    config = malloc(sizeof(config_slave_t));
    memcpy(config->ip_master, buf, SIZE_IP);
    config->ip_master[SIZE_IP -1] = '\0';

    close(fd);
    return 0;
}

int init_config_master(config_master_t *config, char filename[]) 
{
    int fd = open(filename, O_WRONLY);

    if(fd == -1) 
        return 1;

    char buf[MAXBUF];
    int n = read(fd, buf, MAXBUF);
    config = malloc(sizeof(config_master_t));
    char i = 0;

    
}

int **split(char * str, int *size)
{
    int s = 0;
}