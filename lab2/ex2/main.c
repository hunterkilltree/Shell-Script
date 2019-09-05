#include <stdio.h>
#include "readline.h"

int main(int argc, char const *argv[]) {
    char str[100], *pstr;
    int ans;
    FILE *f;

    f = fopen("text.txt", "r");
    if (f == NULL) {
       printf("Error");
       return 1;
    }

    pstr = fgets(str, 150, f);
    ans = read_line(pstr);
    printf("%d\n", ans);

    pstr = fgets(str, 150, f);
    ans = read_line(pstr);
    printf("%d\n", ans);


    pstr = fgets(str, 150, f);
    ans = read_line(pstr);
    printf("%d\n", ans);

    pstr = fgets(str, 150, f);
    ans = read_line(pstr);
    printf("%d\n", ans);

    fclose(f);

    return 0;
}
