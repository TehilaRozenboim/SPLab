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

void string_printer(char *array, void (*func_ptr) (char)) {
    int i = 0;
    while(array[i] != '\0') {
        (*func_ptr)(array[i]);
        printf(" ");
        i++;
    }
    printf("\n");
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