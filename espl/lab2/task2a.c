#include <stdio.h>
#include <string.h>
#include <ctype.h>

void echo_printer(char c) {
	if(c != '\n') {
		printf("%c ", c);
	}
}

void ascii_printer(char c) {
	if(c != '\n') {
		printf("%d ", c);
	}
}

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

void lower_to_upper_printer(char c) {
    if(c != '\n'){
        char output;
        if (isupper(c))
            output = tolower(c);
        else
            output = toupper(c);
        printf("%c", output);
    }
}

int main(int argc, char **argv){
}