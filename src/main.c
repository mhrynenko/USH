#include "../inc/ush.h"


void ctrl_c_signal() {
    printf("\n");
    (void)signal(SIGINT, SIG_DFL);
}
 

int main() {
    // if (setpgid (ush_pgid, ush_pgid) < 0) {
    //     perror ("Couldn't put the shell in its own process group");
    //     exit (1);
    // }
    char *line;
    while (true) {
//        signal(SIGINT, ctrl_c_signal);

//        if (isatty(fileno(stdin))) {
            printf("u$h> ");
//        }
        line = read_input_line();
        // signal(SIGINT, ctrl_c_signal);
        mx_proccess_line(line);
    }

    free(line);
    return 0;
}


