#include <stdio.h> 
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
  
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

struct fun_desc {
	char *name;
	void (*fun)(char);
};

void menu() {
	char *input = malloc(10);
    int menuNumber;
	struct fun_desc menuArr[] = {
		{"echo printer", echo_printer},
		{"ASCII printer", ascii_printer},
		{"binary printer", binary_printer},
		{"lower to upper", lower_to_upper_printer},
	};

	printf("Please enter a string (0<size<=10):\n");
	string_reader(input);
	printf("Please choose printer type:\n");
	for(int i = 0; i < 4; i++) {
		printf("%d) %s\n", i, menuArr[i].name);
	}
    printf("%d) %s\n", 4, "bitwise or");
	printf("\n");
	printf("Option: ");
    scanf("%d", &menuNumber);
	while( menuNumber != EOF) {
		if(menuNumber < 0 || menuNumber > 4) {
			printf("Please choose number between 0-4.\n");
		}
		else {
            if(menuNumber<4)
			    string_printer(input, menuArr[menuNumber].fun);
            else
                bitwise_or(input);
			}
		printf("Option: ");
        scanf("%d", &menuNumber);
	}
    printf("DONE.\n");
}

int main(int argc, char **argv){
	menu();
}
