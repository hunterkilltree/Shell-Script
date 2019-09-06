// main.c
#include <stdio.h>
#include "readline.h"
#include "findsubstr.h"

int main(int argc, char * argv[]) {
	// Implement mygrep
    // grep filter searches a file for a particular pattern of character,
    // and displays all lines that contain that pattern
    char str[150] = "", *pstr;
    int ans;
    char sub[150] = "";
    // get input from user
    fgets(sub, 150, stdin);

    do {
        pstr = fgets(str, 150, stdin);

        if ( find_sub_string(str, sub) != -1)
            printf("%s\n", pstr);;
    }
    while (pstr != NULL);
    
    return 0;
}


//#endif
