#include "../inc/ush.h"

void unset_proccess (const char *var_name) {
    unsetenv(var_name);
}

int check_unset_args(char **splitted_command) {
    for (int i = 0; splitted_command[i] != NULL; i++) {
        if (splitted_command[i] && splitted_command[i][0] == '-' && splitted_command[i][1]) {
            mx_printerr("unset: bad option: -");
            mx_printerrchar(splitted_command[i][1]);
            mx_printerr("\n");
            return 1;
        }
        if (splitted_command[i] && splitted_command[i][0] == '-' && !splitted_command[i][1]) {
            mx_printerr("unset: not enough arguments\n");
            return 1; 
        }
    }
    return 0;
}

void mx_unset(char **splitted_command) {
    if (check_unset_args (splitted_command) == 1) {
        return;
    }
    for (int i = 1; splitted_command[i] != NULL; i++) {
        unset_proccess(splitted_command[i]);
    }
}
