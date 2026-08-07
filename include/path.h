#ifndef __PATH_H__
#define __PATH_H__

/**
 * @fn int is_relative_path(char path[])
 * @brief Verifie si un chemin depend du repertoire courant ou non
 * @param path le chemin a tester
 * @return 1 si c'est le cas 0 sinon
 */
int is_relative_path(char path[]);

/**
 * @brief Fourni le chemin absolu à partir d'un chemin donné, en vérifiant que le chemin résultant est bien dans le dossier du dirpath
 * @param path le chemin à convertir
 * @param server_path le buffer où stocker le chemin absolu vers le serveur
 * @param dirpath le chemin du dossier du serveur
 * @return int 1 si le chemin est valide et dans le dossier du serveur, 0 sinon
 */
int get_abs_dest_path_from_src_path(const char *path, char *server_path, const char *dirpath);

/**
 * @brief Résout un chemin absolu dans dirpath avec mode nécessitant ou non que le chemin existe déjà
 * @param path chemin logique reçu dans la requête
 * @param server_path buffer de sortie
 * @param dirpath dossier racine autorisé
 * @param require_existing 1 si le chemin doit exister, 0 si le chemin peut être creer
 * @return int 1 si le chemin est autorisé selon le mode, 0 sinon
 */
int get_abs_path_from_src_path(const char *path, char *server_path, const char *dirpath, int require_existing);

#endif