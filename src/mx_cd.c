#include "../inc/ush.h"
#include <sys/stat.h>

int last_occurence( const char *str, char c) {
    if (!*str) {
        return -1;
    }
    int n = last_occurence(str + 1, c);
    return n + (n != -1 || *str == c);
}

char *get_link_in_path(char *path, char *link_to) {
    if (readlink(path, link_to, 1024) != -1) {
        return strdup(&path[last_occurence(path, '/') + 1]);
    }

    int ind = last_occurence(path, '/');
    char *link = strndup(path, ind);
    while (ind != -1) {
        if (readlink(link, link_to, 1024) != -1) {
            char *tmp = strdup(&link[last_occurence(link, '/') + 1]);
            free(link);
            return tmp;
        }
        ind = last_occurence(link, '/');
        char *tmp = link;
        link = strndup(link, ind);
        free(tmp);
    }

    free(link);
    return NULL;
}


void check_cd_flags(char **splitted_command, t_cd *cd){
    int i = 1;

    while (splitted_command[i] && splitted_command[i][0] == '-') { 
        if (!strcmp(splitted_command[i], "-sP")) {
            cd->P = true;
            cd->s = true;
            cd->no_fl = false;
        } 
        else if (!strcmp(splitted_command[i], "-Ps")) {
            cd->P = true;
            cd->s = true;
            cd->no_fl = false;
        }
        else if (!strcmp(splitted_command[i], "-P")) {
            cd->P = true;
            cd->no_fl = false;
        }
        else if (!strcmp(splitted_command[i], "-s")) {
            cd->s = true;
            cd->no_fl = false;
        }
        else if (!strcmp(splitted_command[i], "-")) {
            cd->hyphen = true;
            cd->no_fl = false;
        }
        else if (strcmp(splitted_command[i], "-sP") && strcmp(splitted_command[i], "-Ps") && strcmp(splitted_command[i], "-P") && strcmp(splitted_command[i], "-s") && strcmp(splitted_command[i], "-")) {
            if (cd->hyphen || cd->P || cd->s)  {
                cd->no_fl = true;
                cd->s = false;
                cd->P = false;
            }
        }
        i++;
    }
}

void cd_main_proccess (char *to, t_cd cd) {
    char *buffer = (char *)malloc(1024 * sizeof(char));
    char *error = (char *)malloc(1024 * sizeof(char));
    char *path = (char *)malloc(1024 * sizeof(char));

    if (to == NULL && !cd.hyphen) {
        strcpy(buffer, getenv("PWD"));
        chdir(getenv("HOME"));
        setenv("PWD", getenv("HOME"), 1);
        setenv("OLDPWD", buffer, 1);
        free(buffer);
        free(error);
        free(path);
        return;
    }

    //Без флагов
    if (cd.no_fl && to != NULL) {
        strcpy(buffer, getenv("PWD"));

        //если указан полный путь
        if (to[0] == '/') {
            if (chdir(to) != 0) {
                strcat(error, "cd: ");
                strcat(error, to);
                perror(error);
                free(buffer);
                free(error);
                free(path);
                return;
            }
            setenv("OLDPWD", getenv("PWD"), 1);
            setenv("PWD", to, 1);
        }
        //если указана просто папка
        else {
            strcat(buffer, "/");
            strcat(buffer, to);
            
            char link_to[1024] = {'\0'};
            char *link = get_link_in_path(buffer, link_to);
            
            if (chdir(buffer) != 0) {
                strcat(error, "cd: ");
                strcat(error, to);
                perror(error);
                free(link);
                free(buffer);
                free(error);
                free(path);
                return;
            }
            setenv("OLDPWD", getenv("PWD"), 1);

            char *pwd = getcwd(NULL, 0);
            if (link) {
                pwd = mx_replace_substr(pwd, link_to, link);
            }
            
            setenv("PWD", pwd, 1);
            free(pwd);
            free(link);
        }

        free(buffer);
        free(error);
        free(path);
        return;
    }
    //Флаг -
    else if (cd.hyphen) {
        strcpy(buffer, getenv("PWD"));  
        chdir(getenv("OLDPWD"));
        setenv("PWD", getenv("OLDPWD"), 1);
        setenv("OLDPWD", buffer, 1);
        free(buffer);
        free(error);
        free(path);
        return;       
    }
    //Флаг -P
    else if (cd.P) {
        realpath(to, buffer);
        if (chdir(buffer) != 0) {
            strcat(error, "cd: ");
            strcat(error, to);
            perror(error);
            free(buffer);
            free(error);
            free(path);
            return;
        }
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", buffer, 1);
        free(buffer);
        free(error);
        free(path);
        return;       
    }
    //Флаг -s
    else if (cd.s) {
        char link_to[1024];
        if (get_link_in_path(to, link_to)) {
            mx_printerr("cd: not a directory: ");
            mx_printerr(to);
            mx_printerr("\n");
            free(buffer);
            free(error);
            free(path);
            return;
        }
        strcpy(buffer, getenv("PWD"));

        //если указан полный путь
        if (to[0] == '/') {
            if (chdir(to) != 0) {
                strcat(error, "cd: ");
                strcat(error, to);
                perror(error);
                free(buffer);
                free(error);
                free(path);
                return;
            }
            setenv("OLDPWD", getenv("PWD"), 1);
            setenv("PWD", to, 1);
        }
        //если указана просто папка
        else {
            strcat(buffer, "/");
            strcat(buffer, to);
            if (chdir(buffer) != 0) {
                strcat(error, "cd: ");
                strcat(error, to);
                perror(error);
                free(buffer);
                free(error);
                free(path);
                return;
            }
            setenv("OLDPWD", getenv("PWD"), 1);
            setenv("PWD", getcwd(path, 1024), 1); //buffer, 1);
        }
        
        free(buffer);
        free(error);
        free(path);
        return;   
    }
    
    free(buffer);
    free(error);
    free(path);
}

void mx_cd (char **splitted_command) {
    t_cd cd = {true, false, false, false};
    check_cd_flags (splitted_command, &cd);

    //Если нет параметров,то вызываем "cd"
    if (arr_size(splitted_command) <= 1) {
        cd_main_proccess(NULL, cd);
        return;
    }

    char *argument = NULL;
    for (int z = 1; splitted_command[z] != NULL; z++) {
        if (!strcmp(splitted_command[z], "-sP") || !strcmp(splitted_command[z], "-Ps") || !strcmp(splitted_command[z], "-P") || !strcmp(splitted_command[z], "-s") || !strcmp(splitted_command[z], "-")) {
            continue;
        }
        else {
            if (argument != NULL) {
                argument = mx_strjoin(argument, splitted_command[z]);
            }
            else {
                argument = strdup(splitted_command[z]);
            }
            argument = mx_strjoin(argument, " ");
        }
    }
    argument = mx_strtrim(argument);
    cd_main_proccess(argument, cd);
    free(argument);
}

