#include <stdio.h>

extern int funcA(char *ch);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stdout, "provide parameter \n");
        return 0;
    }

    int res = funcA(argv[1]);
    printf("solution: %d\n", res);

    return 0;
}