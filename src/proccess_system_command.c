#include "../inc/ush.h"

void proccess_system_command(char **splitted_command) {
    char *path = find_command_path(splitted_command[0]);
    if (path) {
        int pid = fork();
        if (!pid) {
            execv(path, splitted_command);
            mx_printerr("ush: ");
            perror(path);
            return;
        } else {
            pid_t w = waitpid(pid, NULL, 0);
            if (w == -1) {
                perror("ush");
                return;
            }
        }
    } else {
        if (errno == EACCES) {
            mx_printerr("ush: permission denied: ");
            mx_printerr(splitted_command[0]);
            mx_printerr("\n");
        } else if (errno == ENOTDIR) {
            mx_printerr("ush: not a directory: ");
            mx_printerr(splitted_command[0]);
            mx_printerr("\n");
        } else if (splitted_command[0][0] == '/') {
            mx_printerr("ush: no such file or directory: ");
            mx_printerr(splitted_command[0]);
            mx_printerr("\n");
        } else {
            mx_printerr("ush: command not found: ");
            mx_printerr(splitted_command[0]);
            mx_printerr("\n");
        }
    }
}
