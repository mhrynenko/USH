#include "../inc/ush.h"

void check_echo_flags(char **splitted_command, t_echo *echo) {
    if (splitted_command[1][0] == '-' ) {
        for (int j = mx_strlen(splitted_command[1]); j >= 1; j--) {
            if (splitted_command[1][j] == 'n') {
                echo->n = true;
                echo->no_fl = false;
            }
            else if (splitted_command[1][j] == 'e') {
                if (!echo->E) {
                    echo->e = true;
                    echo->no_fl = false;
                }
            }
            else if (splitted_command[1][j] == 'E') {
                if (!echo->e) {
                    echo->E = true;
                    echo->no_fl = false;
                }
            }
        }
    }      
}

char *replace_slash(char *str) {
    char *result = (char *)malloc(mx_strlen(str) + 1);
    int len = 0;

    for (int i = 0; i <= mx_strlen(str); i++) {
        if (str[i] == '\\' && str[i + 1] == '\\') {
            i++;
        }
        if (str[i] == '\\' && str[i + 1] == 'e') {
            if (str[i + 2] != '\\') {
                (i) += 3;
            }
            else {
                (i) += 2;
            }
        }
        result[len] = str[i];
        len++;
    }
    result[len] = '\0';
    return result;
}

char *mx_replace_escape(char *arg, t_echo echo) {
    char *sequenses[] = {"\\a","\\b","\\f","\\n","\\r","\\t","\\v", NULL};
    char *escape[] = {"\a","\b","\f","\n","\r","\t","\v", NULL};
    char *result = mx_strnew(ARG_MAX);
    int index = 0;
    if ((index = mx_get_substr_index(arg, "\\c")) >= 0) {
        strncpy(result, arg, index);
        echo.n = true;
    }
    else {
        strcpy(result, arg);
    }
    char *tmp = replace_slash(result);
    for (int i = 0; sequenses[i] != NULL; i++) {
        if (strstr(tmp, sequenses[i])) {
            tmp = mx_replace_substr(tmp, sequenses[i], escape[i]);
        }
    }
    free(result);
    return tmp;
}

void echo_main_proccess(char **splitted_command, t_echo echo) {
    int t = 1;
    char *output;
    if (!echo.no_fl) {
        t = 2;
    }
    for (int i = t; splitted_command[i] != NULL; i++) {
        output = splitted_command[i];
        if (!echo.E && strstr(splitted_command[i], "\\")) {
            output = mx_replace_escape(splitted_command[i], echo);
        }
        printf("%s", output);
        if (strstr(splitted_command[i],"\\c")) {
            break;
        }
        if (splitted_command[i + 1]) {
            printf(" ");
        }
    }
    if(!echo.n) {
        printf("\n");
    }
}

void mx_echo(char **splitted_command) {
    if (arr_size(splitted_command) == 1) {
        printf("\n");
        return;
    }
    t_echo echo = {true, false, false, false, false};
    check_echo_flags(splitted_command, &echo);
    echo_main_proccess(splitted_command, echo);
}


