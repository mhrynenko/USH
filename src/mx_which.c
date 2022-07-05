#include "../inc/ush.h"

char **list_builtin = (char *[]){"alias", "autoload", "bg", "bindkey", "break", "builtin", "bye", "cap", "cd", "chdir", "clone",
    "command", "comparguments", "compcall", "compctl", "compdescribe", "compfiles", "compgroups", "compquote", "comptags", "comptry",
    "compvalues", "continue", "continue", "declare", "dirs", "disable", "disown", "echo", "echotc", "echoti", "emulate", "enable",
    "eval", "exec", "exit", "export", "false", "fc", "fg", "functions", "getcap", "getln", "getopts", "hash", "history", "integer",
    "jobs", "kill", "let", "limit", "local", "log", "logout", "noglob", "popd", "print", "printf", "pushd", "pushln", "pwd", "read",
    "readonly", "rehash", "return", "sched", "set", "setcap", "setopt", "shift", "source", "stat", "suspend", "test", "times", "trap",
    "true", "ttyctl", "type", "typeset", "ulimit", "umask", "unalias", "unfunction", "unhash", "unlimit", "unset", "unsetopt", "vared",
    "wait", "whence", "where", "which", "zcompile", "zformat", "zftp", "zle", "zmodload", "zparseopts", "zprof", "zpty", "zregexparse",
    "zsocket", "zstyle", "ztcp", NULL};

bool is_builtin_command(char *command) {
    return mx_binary_search(list_builtin, arr_size(list_builtin), command) != -1;
}

bool is_command_exists(char *command) {
    return find_command_path(command) || is_builtin_command(command);
}

void mx_which(char **splitted_command) {
    setenv("?", "0", 1);
    int i = 1;
    t_which flags = {false, false};
    while (splitted_command[i] && splitted_command[i][0] == '-') {
        if (!strcmp(splitted_command[i], "-a")) {
            flags.a = true;
        } else if (!strcmp(splitted_command[i], "-s")) {
            flags.s = true;
        } else {
            mx_printerr("which: bad option: ");
            mx_printerr(splitted_command[i]);
            mx_printerr("\n");
        }
        i++;
    }
    
    while (splitted_command[i]) {
        bool is_builtin = is_builtin_command(splitted_command[i]);
        char *path = find_command_path(splitted_command[i]);
        if (path) {
            int j = mx_get_char_index(path, '/');
            while(mx_get_char_index(&path[j + 1], '/') != -1) {
                j += mx_get_char_index(&path[j + 1], '/') + 1;
            }
            char *command = strdup(&path[j + 1]);
            if (is_builtin && !flags.s) {
                printf("%s: shell built-in command\n", command);
                if (flags.a) {
                    printf("%s\n", path);
                }
            } else if (!flags.s) {
                printf("%s\n", path);
            }
            free(command);
            free(path);
        } else {
            if (is_builtin) {
                printf("%s: shell built-in command\n", splitted_command[i]);
            } else {
                if (!flags.s) {
                    mx_printerr(splitted_command[i]);
                    mx_printerr(" not found\n");
                }
                setenv("?", "1", 1);
            }
        }
        i++;
    }
}
