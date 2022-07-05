#include "../inc/ush.h"

char *find_command_path(char *command) {
    if (command[0] == '/') {
        DIR *dir = opendir(command);
        if (!dir) {
            if (errno == ENOTDIR && !access(command, F_OK)) {
                return strdup(command);
            }
            return NULL;
        }
        execvp(command, NULL);
        closedir(dir);
        return NULL;
    }
    
    char **paths = mx_strsplit(getenv("PATH"), ':');
    if (!paths) {
        return NULL;
    }
    for (int i = 0; paths[i] != NULL; i++) {
        char *fullpath = strdup(paths[i]);
        fullpath = mx_strjoin(fullpath, "/");
        fullpath = mx_strjoin(fullpath, command);
        struct stat st;
        if (!stat(fullpath, &st)) {
            return fullpath;
        }
        free(fullpath);
    }
    
    mx_del_strarr(&paths);
    return NULL;
}
