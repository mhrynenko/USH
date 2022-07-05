#include "../inc/ush.h"

char *read_input_line() {
    int str_pos = 0;
    int str_size = 2;
    char *command_char = (char *)malloc(str_size * sizeof(char));
    
    while (true) {
        char c = getchar();

        if (c == '\n') {
            command_char[str_pos] = '\0';
            break;
        }

        command_char[str_pos] = c;
        str_pos++;

        if (str_pos >= str_size) {
            str_size = str_size * 2;
            command_char = realloc(command_char, str_size);
        }
    }

    return command_char;
}
