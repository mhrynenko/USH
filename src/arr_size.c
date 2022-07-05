#include "../inc/ush.h"

int arr_size (char **arr) {
    int len = 0;
    while (arr[len] != NULL) {
        len++;
    }
    return len;
}
