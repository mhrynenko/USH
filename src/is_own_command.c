#include "../inc/ush.h"

char *list_of_own_commands[] = {"cd", "echo", "env", "exit", "export", "fg", "pwd", "unset", "which", NULL};

bool is_own_command(char *command) {
    return mx_binary_search(list_of_own_commands, arr_size(list_of_own_commands), command) != -1;
}
