#include <stdio.h>
#include "findsubstr.h"

int main(int argc, char const *argv[]) {
    char str[100] = "";
    char sub[100] = "";
    int ans;

    gets(str);
    gets(sub);
    ans = find_sub_string(str, sub);

    printf("%d\n", ans);

    return 0;
}
