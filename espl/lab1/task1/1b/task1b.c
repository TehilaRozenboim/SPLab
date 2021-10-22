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

void char2binary(char c,int binary[8]){
    int dec = c;
    for(int i=7;i>=0;i--){
        binary[i]=dec % 2;
        dec = dec / 2;
    }
    for(int i=0;i<8;i++){
        printf("%d", binary[i]);
    }
    printf(" ");
}

int main(int argc, char **argv){
    char c = fgetc(stdin);
    if (argc == 1) {
        char2ascii(c);
        return 0;
    }
    if (strcmp(argv[1], "-b") == 0){
        while(c != EOF){
            if (c == '\n'){
                printf("\n");
            } else {
                int binary[8];
                char2binary(c,binary);
            }
            c = fgetc(stdin);
        }
    }
    return 0;
}