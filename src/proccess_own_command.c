#include "../inc/ush.h"

void proccess_own_command(char **splitted_command) {
    int args_size = 0;
    char **args = 0;
    for (int i = 0; splitted_command[i]; i++) {
        int len = strlen(splitted_command[i]);
        if (len >= 3 && splitted_command[i][0] == '$' && splitted_command[i][1] == '(') {
            char *command;
            if (splitted_command[i][len - 1] == ')') {
                command = strndup(&splitted_command[i][2], len - 3);
                if (!is_command_exists(command)) {
                    free(command);
                    mx_printerr("ush: command not found: ");
                    mx_printerr(command);
                    mx_printerr("\n");
                    continue;
                }
            } else {
                command = strdup(&splitted_command[i++][2]);
                if (!is_command_exists(command)) {
                    free(command);
                    mx_printerr("ush: command not found: ");
                    mx_printerr(command);
                    mx_printerr("\n");
                    continue;
                }
                while (splitted_command[i] && splitted_command[i][strlen(splitted_command[i]) - 1] != ')') {
                    command = mx_strjoin(command, " ");
                    command = mx_strjoin(command, splitted_command[i]);
                    i++;
                }
                if (splitted_command[i]) {
                    char *tmp = strndup(splitted_command[i], strlen(splitted_command[i]) - 1);
                    command = mx_strjoin(command, " ");
                    command = mx_strjoin(command, tmp);
                    free(tmp);
                } else {
                    mx_printerr("ush: bad substitution\n");
                    free(command);
                    continue;
                }
            }

            FILE *stream = popen(command, "r");
            if (!stream) {
                free(command);
                perror("ush");
                continue;
            }
            
            free(command);

            char buff[1024] = {'\0'};
            if (read(fileno(stream), buff, 1024) < 0) {
                pclose(stream);
                perror("ush");
                continue;
            }

            char **returned_values = mx_strsplit(buff, '\n');
            for (int j = 0; returned_values[j]; j++) {
                args = push_back_str(args, &args_size, returned_values[j]);
            }

            pclose(stream);
            mx_del_strarr(&returned_values);
        } else if (len >= 3 && splitted_command[i][0] == '$' && splitted_command[i][1] == '{') {
            if (splitted_command[i][len - 1] != '}') {
                mx_printerr("ush: bad substitution\n");
                mx_del_strarr(&args);
                return;
            } else {
                char *name = strndup(&splitted_command[i][2], len - 3);
                char *value = strdup(getenv(name));
                if (value) {
                    args = push_back_str(args, &args_size, value);
                    free(value);
                }
                free(name);
            }
        } else {
            args = push_back_str(args, &args_size, splitted_command[i]);
        }
    }

    if (!strcmp(args[0], "env")) {
        mx_env(args);
    }
    else if (!strcmp(args[0], "cd")) {
        mx_cd(args);
    }
    else if (!strcmp(args[0], "pwd")) {
        mx_pwd(args);
    }
    else if (!strcmp(args[0], "which")) {
        mx_which(args);
    }
    else if (!strcmp(args[0], "echo")) {
        mx_echo(args);
    }
    else if (!strcmp(args[0], "export")) {
        mx_export(args);
    }
    else if (!strcmp(args[0], "unset")) {
        mx_unset(args);
    }
    else if (!strcmp(args[0], "exit")) {
        mx_exit(args);
    }
    else if (!strcmp(args[0], "fg")) {
        printf("FG\n");
        //mx_fg();
    }
    
    mx_del_strarr(&args);
}
