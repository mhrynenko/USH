#include "../inc/ush.h"
void mx_printerrchar(char c) {
    write(2, &c, 1);
}

void read_flags(char flag, t_pwd *pwd) {
    switch (flag) {
        case 'P':      
            pwd->P = true;
            pwd->L = false;
            break;
        case 'L':
            pwd->L = true;
            pwd->P = false;
            break;
        default:
            mx_printerr("ush: pwd: bad option: -");
            mx_printerrchar(flag);
            mx_printerr("\n");
            pwd->error = true;
            break;
    }
}

void check_pwd_flags(char **splitted_command, t_pwd *pwd) {
    for (int i = 1; splitted_command[i] != NULL; i++) {
        if (pwd->error) {
            break;
        }
        if (arr_size(splitted_command) > i && splitted_command[i][0] == '-') {
            for (int k = 1; splitted_command[i][k] != '\0'; k++) {
                read_flags(splitted_command[i][k], pwd);
                if (pwd->error) {
                    break;
                }
            }
            if (strcmp(splitted_command[i], "-LP") && strcmp(splitted_command[i], "-P") && strcmp(splitted_command[i], "-L") && strcmp(splitted_command[i], "-PL")) {
                pwd->L = false;
                pwd->P = false;
            }
        }
        else {
            pwd->L = true; //-L и буз флага - одно и тоже
        }
    }
}


void pwd_proccess(char *line, t_pwd pwd) {
    if (line != NULL) {
        mx_printerr("pwd: too many arguments\n");
        return;
    }
    char *path_P = (char *)malloc(1024*sizeof(char));
    if (pwd.L) {
        printf("%s\n", getenv("PWD"));
    }
    else {
        printf("%s\n", getcwd(path_P, 1024));
    }
    free(path_P);
}


void mx_pwd (char **splitted_command) {
    t_pwd pwd = {true, false, false};

    check_pwd_flags (splitted_command, &pwd);

    char *argument = NULL;
    for (int z = 1; splitted_command[z] != NULL; z++) {
        if (!strcmp(splitted_command[z], "-LP") || !strcmp(splitted_command[z], "-P") || !strcmp(splitted_command[z], "-L") || !strcmp(splitted_command[z], "-PL") || splitted_command[z][0] == '-') {
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
        argument = mx_strtrim(argument);
    }
    if(!pwd.error) {
        pwd_proccess(argument, pwd);
    }
    free(argument);
}
