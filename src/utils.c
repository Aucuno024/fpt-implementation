#include "utils.h"
#include "csapp.h"
#include <stdint.h>
#include "string.h"
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include "path.h"

#define SPEAKER "Gyro"

int get_endianess() 
{
    static uint32_t one = 1;

    return ((* (uint8_t *) &one) == 0);
}

static int ensure_parent_dirs_for_file(const char *file_path) 
{
    char tmp[MAXLINE];
    char *p;

    if (file_path == NULL) 
        return 0;

    if (snprintf(tmp, sizeof(tmp), "%s", file_path) >= (int)sizeof(tmp)) 
        return 0;
    
    p = tmp;

    if (*p == '/') 
        p++;
    
    for (; *p != '\0'; p++) 
    {
        if (*p != '/') 
            continue;

        *p = '\0';

        if (tmp[0] != '\0' && mkdir(tmp, 0755) != 0 && errno != EEXIST) 
            return 0;

        *p = '/';
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
int write_all_fd(int fd, const uint8_t *buf, size_t len) 
{
    size_t written = 0;

    while (written < len) 
    {
        ssize_t n = rio_writen(fd, (void *)(buf + written), len - written);

        if (n < 0) 
            return 0;
        
        written += (size_t)n;
    }

    return 1;
}

int open_file_r(char path[], int *fd, const char *dirpath)
{
    if (path == NULL || fd == NULL) 
        return 0;
    
    char abs_path[MAXLINE];

    if (!get_abs_dest_path_from_src_path(path, abs_path, dirpath)) 
        return 0;
    
    struct stat st;
    stat(abs_path, &st);

    if(S_ISDIR(st.st_mode))
        return 0;

    return (*fd = open(abs_path, O_RDONLY, 0)) != -1;
}

int open_file_w(char path[], int *fd, const char *dirpath)
{
    if (path == NULL || fd == NULL) 
        return 0;

    char abs_path[MAXLINE];

    if (!get_abs_path_from_src_path(path, abs_path, dirpath, 0)) 
        return 0;

    if (!ensure_parent_dirs_for_file(abs_path)) 
        return 0;
    
    struct stat st;
    stat(abs_path, &st);

    if(S_ISDIR(st.st_mode))
        return 0;

    return (*fd = open(abs_path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) != -1;
}

int write_file_from_content(char path[], const uint8_t *content, size_t content_size)
{
    int fd;
    int ok = 1;

    if (path == NULL || content == NULL) 
        return 0;

    if ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) 
        return 0;

    if (content_size > 0) 
        ok = write_all_fd(fd, content, content_size);

    if (close(fd) < 0) 
        return 0;

    return ok;
}

int write_file_to_dest_dir(char path[], const uint8_t *content, size_t content_size, const char *dirpath)
{
    int fd;
    int ok = 1;

    if (path == NULL || content == NULL || dirpath == NULL) 
        return 0;

    if (!open_file_w(path, &fd, dirpath)) 
        return 0;

    if (content_size > 0) 
        ok = write_all_fd(fd, content, content_size);

    if (close(fd) < 0) 
        return 0;

    return ok;
}


int update(char **contentptr, char *element)
{
    size_t s = strlen(element);
    char *content = *contentptr;

    if(!content)
    {
        #ifdef DEBUG
            printf("%s say \"First element added\"\n", SPEAKER);
        #endif

        content = malloc(s + 1);

        if(!content)
            return 1;

        for(int i = 0; i < s; i++ )
        
            content[i] = element[i];
        
        content[s] = '\0';

        #ifdef DEBUG
            printf("%s say \"content value : %s\"\n", SPEAKER, *content);
        #endif

        return 0;
    }

    size_t sc = strlen(content);
    char * tmp = realloc(content, sc + s + 2);

    if(!tmp)
        return 1;

    content = tmp;
    content[sc] = '\n';

    for(int i = sc + 1; i < sc + s + 1; i++)
        content[i] = element[i - sc - 1];
    
    content[sc + s + 1] = '\0';

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

char **split(char *str, char chr, int size_str, int *size_splited)
{
    char **splited = NULL;
    int size = 0;

    for(int i = 0; i < size_str; i++)
    {
        if(str[i] == chr)
        {
            if(!splited)
            {
                splited = malloc(sizeof(char *));
                splited[size++] = str + i + 1;
            } else 
            {
                splited = realloc(splited, sizeof(char *) * (size + 1));
                splited[size++] = str + i + 1;
            }
        }
    }

    *size_splited = size;

    return splited;
}