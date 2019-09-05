#include <stdio.h>

int read_line(char *str) {

    if (str == NULL)
        return -1;

    int i = 0;
    while(str[i] != '\0' ) {
        i++;
    }
    
    return i - 1;
}
