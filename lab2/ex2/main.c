#include <stdio.h>
#include "readline.h"

int main(int argc, char const *argv[]) {
    char str[100];
    int ans;
    FILE *f;

    f = fopen("text.txt", "r");
    if (f == NULL) {
       printf("Khong mo duoc file array.txt");
       return 1;
    }

    fgets(str, 150, f);
    ans = read_line(str);
    printf("%d\n", ans);

    fgets(str, 150, f);
    ans = read_line(str);
    printf("%d\n", ans);


    fgets(str, 150, f);
    ans = read_line(str);
    printf("%d\n", ans);

    fgets(str, 150, f);
    ans = read_line(str);
    printf("%d\n", ans);

    fclose(f);

    return 0;
}
