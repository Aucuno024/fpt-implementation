#include "csapp.h"
#include <stdint.h>
#include "string.h"
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include "path.h"

/**
 * @brief Vérification si le fichier abs_path a ouvrir est dans le dossier dirpath 
 * @param abs_path le chemin absolu du fichier a ouvrir
 * @param dirpath le chemin du dossier du serveur
 * @return int 1 si le fichier est dans le dossier du serveur, 0 sinon
 */
int is_path_in_dirpath(const char *abs_path, const char * dirpath) 
{
    char server_dir_abs[MAXLINE];

    if (realpath(dirpath, server_dir_abs) == NULL) 
        return 0;
    
    size_t dir_len = strlen(server_dir_abs);

    return strncmp(abs_path, server_dir_abs, dir_len) == 0 && (abs_path[dir_len] == '/' || abs_path[dir_len] == '\0');
}


/**
 * @brief Construit un chemin candidat sous dirpath sans le réduire
 */
int build_candidate_path(const char *path, char *candidate_path, size_t candidate_size, const char *dirpath) 
{
    const char *suffix;
    size_t dir_len;

    if (path == NULL || candidate_path == NULL || dirpath == NULL || candidate_size == 0) 
        return 0;

    dir_len = strlen(dirpath);

    if (dir_len == 0) 
        return 0;

    if (path[0] == '/') 
    {
        suffix = path;

        while (*suffix == '/') 
            suffix++;

        if (snprintf(candidate_path, candidate_size, "%s%s", dirpath, suffix) >= (int)candidate_size) 
            return 0;
        
        return 1;
    }

    if (snprintf(candidate_path, candidate_size, "%s%s%s", dirpath, (dirpath[dir_len - 1] == '/') ? "" : "/", path) >= (int)candidate_size) 
        return 0;
    
    return 1;
}

int get_abs_path_from_src_path(const char *path, char *server_path, const char *dirpath, int require_existing) 
{
    char candidate_path[MAXLINE];

    if (path == NULL || server_path == NULL || dirpath == NULL) 
        return 0;
    

    if (!build_candidate_path(path, candidate_path, sizeof(candidate_path), dirpath)) 
        return 0;
    

    if (require_existing) 
    {
        if (realpath(candidate_path, server_path) == NULL)
        {
            server_path[0] = '\0';
            return 0;
        }

        if (!is_path_in_dirpath(server_path, dirpath)) 
        {
            server_path[0] = '\0';
            return 0;
        }

        return 1;
    }

    // Si le fichier n'est pas obligatoirement existant
    if (!require_existing) 
    {
        char root_abs[MAXLINE];
        char rel_path[MAXLINE];
        char *cursor;
        int has_component = 0;

        if (realpath(dirpath, root_abs) == NULL) 
            return 0;

        if (path[0] == '/') 
        {
            const char *suffix = path;
            while (*suffix == '/') 
                suffix++;
            
            if (snprintf(rel_path, sizeof(rel_path), "%s", suffix) >= (int)sizeof(rel_path)) 
                return 0;
            
        } else
        {
            if (snprintf(rel_path, sizeof(rel_path), "%s", path) >= (int)sizeof(rel_path)) 
                return 0;
        }

        if (rel_path[0] == '\0')  
            return 0;

        if (rel_path[strlen(rel_path) - 1] == '/')  
            return 0;

        if (snprintf(server_path, MAXLINE, "%s", root_abs) >= MAXLINE)  
            return 0;

        cursor = rel_path;

        while (*cursor != '\0') 
        {
            char *next_sep = strchr(cursor, '/');
            size_t comp_len = next_sep ? (size_t)(next_sep - cursor) : strlen(cursor);

            if (comp_len == 0) 
            {
                cursor = next_sep ? (next_sep + 1) : (cursor + strlen(cursor));
                continue;
            }

            if ((comp_len == 1 && cursor[0] == '.') || (comp_len == 2 && cursor[0] == '.' && cursor[1] == '.')) 
                return 0;
            

            if (snprintf(server_path + strlen(server_path), MAXLINE - strlen(server_path), "/%.*s", (int)comp_len, cursor) >= (int)(MAXLINE - strlen(server_path))) 
                return 0;
            

            has_component = 1;
            cursor = next_sep ? (next_sep + 1) : (cursor + comp_len);
        }

        if (!has_component) 
            return 0;
        

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
void convert_to_abs_path(const char *path, char *abs_path, const char *dirpath) 
{
    if (path == NULL || abs_path == NULL) 
    {
        abs_path[0] = '\0'; // chemin invalide
        return;
    }

    if (!get_abs_path_from_src_path(path, abs_path, dirpath, 1)) 
        abs_path[0] = '\0'; // chemin invalide
    
}

int get_abs_dest_path_from_src_path(const char *path, char *server_path, const char *dirpath) 
{
    return get_abs_path_from_src_path(path, server_path, dirpath, 1);
}

int is_relative_path(char path[])
{
    return path[0] == '~' || path[0] == '/'? 0: 1;
}