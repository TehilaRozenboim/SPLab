#include <stdio.h>
#include <string.h>

void char2ascii(char c){
    while(c != EOF){
        if (c == '\n'){
            printf("\n");
        } else {
            printf("%d ", c);
        }
        c = fgetc(stdin);
    }
}

int main(int argc, char **argv){
    char c = fgetc(stdin);
    char2ascii(c);
    return 0;
}