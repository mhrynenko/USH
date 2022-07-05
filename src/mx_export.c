#include "../inc/ush.h"

extern char **environ;

void mx_export(char **splitted_command) {
    int size = arr_size(splitted_command);
    if (size == 1) {
        int env_size = 0;
        char **env = 0;
        char **p = environ;
        while (*p) {
            char *str = strdup(*p);
            
            int j = mx_get_char_index(str, '=');
            char **value = mx_split_line(&str[j + 1], ' ');
            if (!*value) {
                str = mx_strjoin(str, "\'\'");
            } else if (arr_size(value) > 1) {
                char *temp = str;
                str = strndup(temp, j + 1);
                str = mx_strjoin(str, "\'");
                str = mx_strjoin(str, &temp[j + 1]);
                str = mx_strjoin(str, "\'");
                free(temp);
            }
            
            env = push_back_str(env, &env_size, str);
            p++;
            free(str);
            free(value);
        }
        quick_sort_char_arr(env, 0, env_size - 1);
        for (int i = 0; i < env_size; i++) {
            printf("%s\n", env[i]);
        }
    } else {
        int i = 1, j;
        while (i < size && (j = mx_get_char_index(splitted_command[i], '=')) != -1) {
            char *name = strndup(splitted_command[i], j);
            char *value = strdup(&splitted_command[i][j + 1]);
            setenv(name, value, 1);
            free(name);
            free(value);
            i++;
        }
    }
}
