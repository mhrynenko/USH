#include "../inc/ush.h"

//Передаем строку, введенную пользователем
void mx_proccess_line(char *line) {
    char **splited_line_by_semicolumn = mx_strsplit(line, ';');

    //ибавляемся от пробелов до или после команды, иначе команда не распознается
    for (int i = 0; splited_line_by_semicolumn[i] != NULL; i++) {
        splited_line_by_semicolumn[i] = mx_strtrim(splited_line_by_semicolumn[i]);
    }

    //Обрабатываем каждую команду строки, разделенной ;
    for (int i = 0; splited_line_by_semicolumn[i] != NULL; i++) {
        wordexp_t p;
        
        if (wordexp(splited_line_by_semicolumn[i], &p, 0)) {
            mx_printerr("ush: bad substitution\n");
            continue;
        }
        
        int size = 0;
        char **splitted_command = 0;
        for (int j = 0; p.we_wordv[j]; j++) {
            splitted_command = push_back_str(splitted_command, &size, p.we_wordv[j]);
        }
        
        if (mx_count_substr(splited_line_by_semicolumn[i], "${OLDPWD}") > 0 ||
            mx_count_substr(splited_line_by_semicolumn[i], "$OLDPWD") > 0) {
            splitted_command = push_back_str(splitted_command, &size, getenv("OLDPWD"));
        }
        
        if (is_own_command(splitted_command[0])) {
            proccess_own_command(splitted_command);
        } else {
            proccess_system_command(splitted_command);
        }
        
        wordfree(&p);
        mx_del_strarr(&splitted_command);
    }
}
