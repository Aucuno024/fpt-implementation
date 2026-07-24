#ifndef __CONFIG_LOADER_H__
#define __CONFIG_LOADER_H__
#include "csapp.h"
#define SIZE_IP 13

typedef struct {
    char ip_master[13];
} config_slave_t;

typedef struct {
    char **ip_slaves;
    int number;
} config_master_t;

#endif