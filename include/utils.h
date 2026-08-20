#ifndef __UTILS_H__
#define __UTILS_H__
#include "csapp.h"

#ifndef DEFAULT_SERVER_DIR
#define DEFAULT_SERVER_DIR "./serverdir/"
#endif

#ifndef DEFAULT_CLIENT_DIR
#define DEFAULT_CLIENT_DIR "./clientdir/"
#endif

/**
 * @brief Retourne l'endianness de la machine
 * @return int 0 si little endian, 1 si big endian
 */
int get_endianess();

/**
 * @fn int open_file_r(char path[], int *fd)
 * @brief Ouvre un fichier a un chemin donnee dans fd, en verifiant que le chemin est valide et dans le dossier dirpath
 * @param path le chemin vers le fichier
 * @param fd le parametre recevant le descripteur
 * @param dirpath le chemin du dossier du serveur
 * @return 1 si tout s'est bien passé 0
 */
int open_file_r(char path[], int *fd, const char *dirpath);

/**
 * @brief Ouvre un fichier en ecriture dans dirpath, en autorisant la creation
 * @param path le chemin logique vers le fichier
 * @param fd le parametre recevant le descripteur
 * @param dirpath le chemin du dossier autorise
 * @return 1 si tout s'est bien passe, 0 sinon
 */
int open_file_w(char path[], int *fd, const char *dirpath);

/**
 * @brief Ecrit le contenu dans un fichier a un chemin donnee
 * @param path le chemin vers le fichier
 * @param content le contenu a ecrire dans le fichier
 * @param content_size la taille exacte du contenu en octets
 * @return 1 si tout s'est bien passé 0 sinon
 */
int write_file_from_content(char path[], const uint8_t *content, size_t content_size);

/**
 * @brief Ecrit le contenu dans un fichier a un chemin donnee dans le dossier destdir
 * @param path le chemin vers le fichier
 * @param content le contenu a ecrire dans le fichier
 * @param content_size la taille exacte du contenu en octets
 * @param dirpath le chemin du dossier de destination
 * @return 1 si tout s'est bien passé 0 sinon
 */
int write_file_to_dest_dir(char path[], const uint8_t *content, size_t content_size, const char *dirpath);


/**
 * @fn int list_dir(char *path, char *content) 
 * @brief Liste le contenu d'un repetoire a une adresse donnee dans un pointeur passe en parametre
 * @param path l'adresse du repertoire
 * @param content le pointeur qui va obtenir le contenu sous la forme 'content\ncontent2'
 * @return 0 si réussi sans probleme 1 sinon
 */
int list_dir(char *path, char **content);

/**
 * @fn int update(char **content, char *element)
 * @brief Concatene dynamiquement un tableau de caractere a un autre en les separant de \n
 * @param content un pointeur vers le tableau de caractere principal
 * @param element le tableau de caractere a ajouter
 * @return 1 si erreur 0 sinon
 */
int update(char **content, char *element);

/**
 * @fn char **split(char *str, char chr, int size_str, int *size_splited)
 * @brief Cree un tableau de pointeur vers toutes les chaines de caractères suivant chr dans str
 * @param str la chaine de caractere a split
 * @param chr le caractere de split
 * @param size_str la taille de la chaine
 * @param size_splited la taille du tableau de sous chaine creee
 * @return NULL si il n'y a aucune sous chaine, un tableau de sous chaine sinon
 */
char **split(char *str, char chr, int size_str, int *size_splited);
#endif
