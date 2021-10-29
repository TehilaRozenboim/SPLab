#include <stdio.h>
#include <string.h>
#include <ctype.h>


void binary_printer(char c) {
    if(c != '\n') {
        int dec = c;
        int binary[8];
        for (int i = 7; i >= 0; i--) {
            binary[i] = dec % 2;
            dec = dec / 2;
        }

        for(int i = 0; i < 8; i++)
            printf("%d", binary[i]);
        printf(" ");
    }
}

void string_reader(char* s) {
    fgets(s, 11, stdin);
}

void bitwise_or(char* s){
	int i = 1;
	int dec = (int)s[0];
	while(s[i] != '\0') {
		dec = dec | (int)s[i];
		i++;
	}
	binary_printer(dec);
	printf("\n");
}

int main(int argc, char **argv){
    bitwise_or("abd");
    return 0;
}