#include "../inc/ush.h"

extern char **environ;

void print_env(char **env) {
    if (env) {
        while (*env) {
            printf("%s\n", *env);
            env++;
        }
    }
}

void mx_execve(const char *proccess_name, char *path, char **args, char **env) {
    execve(path, args, env);
    mx_printerr(proccess_name);
    mx_printerr(": ");
    perror(path);
}

void mx_env(char **splitted_command) {
    int env_size = 0;
    char **env = 0;
    char **p = environ;
    while (*p) {
        env = push_back_str(env, &env_size, *p);
        p++;
    }

    char *path = 0;
    int size = arr_size(splitted_command);
    
    int i = 1;
    while (i < size && splitted_command[i][0] == '-') {
        if (!strcmp(splitted_command[i], "-i")) {
            mx_del_strarr(&env);
            env_size = 0;
        } else if (!strcmp(splitted_command[i], "-u")) {
            if (++i < size) {
                for (int k = 0; k < env_size; k++) {
                    char *name = strndup(env[k], mx_get_char_index(env[k], '='));
                    if (!strcmp(splitted_command[i], name)) {
                        free(env[k]);
                        env[k] = 0;
                    }
                    free(name);
                }
            } else {
                mx_printerr("env: option requires an argument -- u\n");
                mx_printerr("usage: env [-i] [-P utilpath] [-u name] [name=value ...] [utility [argument ...]]\n");
                return;
            }
        } else if (!strcmp(splitted_command[i], "-P")) {
            if (++i < size) {
                path = strdup(splitted_command[i]);
            } else {
                mx_printerr("env: option requires an argument -- P\n");
                mx_printerr("usage: env [-i] [-P utilpath] [-u name] [name=value ...] [utility [argument ...]]\n");
                exit(EXIT_FAILURE);
            }
        } else {
            mx_printerr("env: illegal option -- ");
            mx_printerr(splitted_command[i]);
            mx_printerr("\nusage: env [-i] [-P utilpath] [-u name] [name=value ...] [utility [argument ...]]\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    
    int j;
    while (i < size && (j = mx_get_char_index(splitted_command[i], '=')) > 0) {
        char *name = strndup(splitted_command[i], j);
        char *value = strdup(&splitted_command[i][j + 1]);
        
        bool is_exist = false;
        for (int k = 0; k < env_size; k++) {
            int ind = mx_get_char_index(env[k], '=');
            char *tmp = strndup(env[k], ind);
            if (!strcmp(name, tmp)) {
                strcpy(env[k], splitted_command[i]);
                is_exist = true;
                free(tmp);
                break;
            }
            free(tmp);
        }

        if (!is_exist) {
            env = push_back_str(env, &env_size, splitted_command[i]);
        }

        free(name);
        free(value);
        i++;
    }
    
    if (i < size) {
        char *fullpath = 0;
        char **paths = mx_strsplit(getenv("PATH"), ':');
        bool not_found = true;
        for (int k = 0; paths[k]; k++) {
            struct stat st;
            fullpath = strdup(paths[k]);
            fullpath = mx_strjoin(fullpath, "/");
            fullpath = mx_strjoin(fullpath, splitted_command[i]);
            if (!stat(fullpath, &st)) {
                not_found = false;
                break;
            }
        }
        mx_del_strarr(&paths);

        if (not_found) {
            char *tmp = fullpath;
            fullpath = strdup(splitted_command[i]);
            free(tmp);
        }

        int pid = fork();
        if (!pid) {
            if (path) {
                DIR *dir = opendir(path);
                if (dir) {
                    struct dirent *dir_ent;
                    while ((dir_ent = readdir(dir))) {
                        if (!strcmp(dir_ent->d_name, splitted_command[i])) {
                            closedir(dir);
                            path = mx_strjoin(path, "/");
                            path = mx_strjoin(path, splitted_command[i]);
                            mx_execve("env", path, &splitted_command[i], env);
                            free(path);
                            mx_del_strarr(&env);
                            free(fullpath);
                            return;
                        }
                    }
                    closedir(dir);
                }
                mx_execve("env", splitted_command[i], &splitted_command[i], env);
                mx_del_strarr(&env);
                free(fullpath);
                return;
            } else {
                mx_execve("env", fullpath, &splitted_command[i], env);
                mx_del_strarr(&env);
                free(fullpath);
                return;
            }
        } else {
            free(path);
            waitpid(pid, NULL, 0);
        }
        free(fullpath);
    } else {
        print_env(env);
    }
    mx_del_strarr(&env);
}
