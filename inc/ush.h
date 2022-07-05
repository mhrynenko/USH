#ifndef USH_H
#define USH_H

#include "../libmx/inc/libmx.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <wordexp.h>

typedef struct s_cd
{
    bool no_fl;
    bool s;
    bool P;
    bool hyphen;
}              t_cd;

typedef struct s_pwd
{
    bool L;
    bool P;
    bool error;
}              t_pwd;

typedef struct s_which
{
    bool a;
    bool s;
}              t_which;

typedef struct s_echo
{
    bool no_fl;
    bool n;
    bool e;
    bool E;
    bool error;
}              t_echo;

int arr_size (char **arr); //эту функцию можно тоже добавить в либу, а можно оставить тут
void mx_printerrchar(char c);

char *read_input_line();
char **mx_split_line(const char *s, char c);

void mx_env(char **splitted_command);
void mx_cd (char **splitted_command);
void mx_pwd(char **splitted_command);
void mx_which(char **splitted_command);
void mx_unset(char **splitted_command);
void mx_export(char **splitted_command);
void mx_exit(char **splitted_command);
void mx_echo(char **splitted_command);

void proccess_system_command(char **splitted_command);
void proccess_own_command(char **splitted_command);
void mx_proccess_line(char *line);

char *find_command_path(char *command);

bool is_own_command(char *command);
bool is_command_exists(char *command);

char **manage_system_commands (char ***split_buffer);

char **push_back_str(char **arr, int *size, char *str);

#endif
