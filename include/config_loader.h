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

/**
 * @fn init_config_slave(config_slave_t *config, char *filename)
 * @brief ecrit la configuration d'un serveur slave depuis un fichier config dans une strcture
 * @param config un pointeur pour une configuration de serveur
 * @param filename la configuration du serveur
 * @param 0 si réussite 1 si erreur
 */
int init_config_slave(config_slave_t *config, char *filename);

#endif