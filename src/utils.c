#include "utils.h"
#include "csapp.h"
#include <stdint.h>
#include "string.h"
#include <dirent.h>

#define SPEAKER "Gyro"

int get_endianess() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

/**
 * @brief Vérification si le fichier abs_path a ouvrir est dans le dossier dirpath 
 * @param abs_path le chemin absolu du fichier a ouvrir
 * @param dirpath le chemin du dossier du serveur
 * @return int 1 si le fichier est dans le dossier du serveur, 0 sinon
 */
int is_path_in_dirpath(const char *abs_path, const char * dirpath) {
    char server_dir_abs[MAXLINE];
    if (realpath(dirpath, server_dir_abs) == NULL) {
        return 0;
    }
    size_t dir_len = strlen(server_dir_abs);
    return strncmp(abs_path, server_dir_abs, dir_len) == 0 && (abs_path[dir_len] == '/' || abs_path[dir_len] == '\0');
}


/**
 * @brief Construit un chemin candidat sous dirpath sans le réduire
 */
int build_candidate_path(const char *path, char *candidate_path, size_t candidate_size, const char *dirpath) {
    const char *suffix;
    size_t dir_len;

    if (path == NULL || candidate_path == NULL || dirpath == NULL || candidate_size == 0) {
        return 0;
    }

    dir_len = strlen(dirpath);
    if (dir_len == 0) {
        return 0;
    }

    if (path[0] == '/') {
        suffix = path;
        while (*suffix == '/') {
            suffix++;
        }
        if (snprintf(candidate_path, candidate_size, "%s%s", dirpath, suffix) >= (int)candidate_size) {
            return 0;
        }
        return 1;
    }

    if (snprintf(candidate_path, candidate_size, "%s%s%s", dirpath, (dirpath[dir_len - 1] == '/') ? "" : "/", path) >= (int)candidate_size) {
        return 0;
    }
    return 1;
}

/** 
 * @brief Ecrire tout le contenu dans un descripteur de fichier
 * @param fd le descripteur de fichier
 * @param buf le buffer contenant les données à écrire
 * @param len la taille des données à écrire
 * @return int 1 si l'écriture est réussie, 0 sinon
 */
int write_all_fd(int fd, const uint8_t *buf, size_t len) {
    size_t written = 0;

    while (written < len) {
        ssize_t n = rio_writen(fd, (void *)(buf + written), len - written);
        if (n < 0) {
            return 0;
        }
        written += (size_t)n;
    }

    return 1;
}


int get_abs_path_from_src_path(const char *path, char *server_path, const char *dirpath, int require_existing) {
    char candidate_path[MAXLINE];

    if (path == NULL || server_path == NULL || dirpath == NULL) {
        return 0;
    }

    if (!build_candidate_path(path, candidate_path, sizeof(candidate_path), dirpath)) {
        return 0;
    }

    if (require_existing) {
        if (realpath(candidate_path, server_path) == NULL) {
            server_path[0] = '\0';
            return 0;
        }

        if (!is_path_in_dirpath(server_path, dirpath)) {
            server_path[0] = '\0';
            return 0;
        }

        return 1;
    }

    if (!require_existing) {
        char parent_path[MAXLINE];
        char parent_abs[MAXLINE];
        char *last_sep;
        const char *filename;

        last_sep = strrchr(candidate_path, '/');
        if (last_sep == NULL) {
            return 0;
        }

        filename = last_sep + 1;
        if (filename[0] == '\0') {
            return 0;
        }
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
            return 0;
        }
        if (last_sep == candidate_path) {
            parent_path[0] = '/';
            parent_path[1] = '\0';
        } else {
            size_t parent_len = (size_t)(last_sep - candidate_path);
            if (parent_len >= sizeof(parent_path)) {
                return 0;
            }
            memcpy(parent_path, candidate_path, parent_len);
            parent_path[parent_len] = '\0';
        }

        if (realpath(parent_path, parent_abs) == NULL) {
            return 0;
        }

        if (!is_path_in_dirpath(parent_abs, dirpath)) {
            return 0;
        }

        if (snprintf(server_path, MAXLINE, "%s/%s", parent_abs, filename) >= MAXLINE) {
            return 0;
        }

        return 1;
    }

    return 0;
}


/** 
 * @brief Convertir un chemin en chemin absolu depuis le dossier dirpath
 * @param path le chemin à convertir
 * @param abs_path le buffer où stocker le chemin absolu converti
 * @param dirpath le chemin du dossier du serveur
 */
void convert_to_abs_path(const char *path, char *abs_path, const char *dirpath) {
    if (path == NULL || abs_path == NULL) {
        abs_path[0] = '\0'; // chemin invalide
        return;
    }

    if (!get_abs_path_from_src_path(path, abs_path, dirpath, 1)) {
        abs_path[0] = '\0'; // chemin invalide
    }
}
int get_abs_dest_path_from_src_path(const char *path, char *server_path, const char *dirpath) {
    return get_abs_path_from_src_path(path, server_path, dirpath, 1);
}


int open_file_r(char path[], int *fd, const char *dirpath)
{
    if (path == NULL || fd == NULL) {
        return 0;
    }
    char abs_path[MAXLINE];
    if (!get_abs_dest_path_from_src_path(path, abs_path, dirpath)) {
        return 0;
    }
    return (*fd = open(abs_path, O_RDONLY, 0)) != -1;
}

int write_file_from_content(char path[], const uint8_t *content, size_t content_size)
{
    int fd;
    int ok = 1;

    if (path == NULL || content == NULL) {
        return 0;
    }

    if ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        return 0;
    }

    if (content_size > 0) {
        ok = write_all_fd(fd, content, content_size);
    }

    if (close(fd) < 0) {
        return 0;
    }

    return ok;
}

int write_file_to_dest_dir(char path[], const uint8_t *content, size_t content_size, const char *dirpath)
{
    char abs_path[MAXLINE];

    if (path == NULL || content == NULL || dirpath == NULL) {
        return 0;
    }

    if (!get_abs_path_from_src_path(path, abs_path, dirpath, 0)) {
        return 0;
    }

    return write_file_from_content(abs_path, content, content_size);
}

int is_relative_path(char path[])
{
    return path[0] == '~' || path[0] == '/'? 0: 1;
}

int update(char **content, char *element)
{
    size_t s = strlen(element);
    if(!(*content))
    {
        #ifdef DEBUG
            printf("%s say \"First element added\"\n", SPEAKER);
        #endif
        (*content) = malloc(s + 1);
        if(!(*content))
            return 1;
        for(int i = 0; i < s; i++ )
        {
            (*content)[i] = element[i];
        }
        (*content)[s] = '\0';
        #ifdef DEBUG
            printf("%s say \"content value : %s\"\n", SPEAKER, *content);
        #endif
        return 0;
    }
    size_t sc = strlen((*content));
    char * tmp = realloc((*content), sc + s + 2);
    if(!tmp)
        return 1;
    (*content) = tmp;
    (*content)[sc] = '\n';
    for(int i = sc + 1; i < sc + s + 1; i++)
    {
        (*content)[i] = element[i - sc - 1];
    }
    (*content)[sc + s + 1] = '\0';
    #ifdef DEBUG
            printf("%s say \"Content value : %s\"\n", SPEAKER, *content);
    #endif
    return 0;
}

int list_dir(char *path, char **content) 
{
    char server_path[MAXBUF];
    if(!get_abs_dest_path_from_src_path(path, server_path, DEFAULT_SERVER_DIR))
    {
        #ifdef DEBUG
            printf("%s say \"Path non etendu : %s\"\n", SPEAKER, path);
        #endif
        return 1;
    }
    #ifdef DEBUG
            printf("%s say \"Path etendu : %s -> %s\"\n", SPEAKER, path, server_path);
     #endif
    
    struct dirent *de;
    DIR *dr = opendir(server_path);
    if(!dr)
    {
        #ifdef DEBUG
            printf("%s say \"Ne peux pas ouvrir: %s\"\n", SPEAKER, server_path);
        #endif
        int fd = open(server_path, O_RDONLY, 0);
        if(fd != -1)
        {
            #ifdef DEBUG
                printf("%s say \"Fichier ouvert : %d \"\n", SPEAKER, fd);
            #endif
            *content= malloc(sizeof(path) + 1);
            strcpy(*content, path);
            return 0;
        }
        return 1;
    }
     while ((de = readdir(dr)) != NULL)
    {
        #ifdef DEBUG
            printf("%s say \"Dir value : %s\"\n", SPEAKER, de->d_name);
        #endif
        if(update(content, de->d_name))
        {
             #ifdef DEBUG
                printf("%s say \"Erreur: %s\"\n", SPEAKER, *content);
            #endif
            return 1;
        }
    }
    closedir(dr);    
    return 0;
}

