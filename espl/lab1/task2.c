#include <stdio.h>
#include <string.h>
#include <ctype.h> 


void char2ascii(int isInputFile,FILE *inputFile,int isOutputFile, char *outputFileName){
    char inputChar = fgetc(inputFile);
    FILE *outputFile;
    if (isOutputFile)
        outputFile = fopen(outputFileName, "w");
    while(inputChar != EOF){
        if(isOutputFile){
             if (inputChar == '\n'){
                 fputs("\n", outputFile);
             }else {
                fprintf(outputFile, "%d ",inputChar);
             }
             fflush(outputFile);
        }else{
            if (inputChar == '\n'){
                printf("\n");
            }else{
                printf("%d ", inputChar);
            }
            
        }
        inputChar = fgetc(inputFile);  
            
           
    }
    fclose(outputFile);   
}

void getBinaryArray(char c,int binary[8]){
    int dec = c;
    for(int i=7;i>=0;i--){
        binary[i]=dec % 2;
        dec = dec / 2;
    }
}
void char2binary(int isInputFile,FILE *inputFile,int isOutputFile, char *outputFileName){
    char inputChar = fgetc(inputFile);
    FILE *outputFile;
    if (isOutputFile)
        outputFile = fopen(outputFileName, "w");
    while(inputChar != EOF){
        int binary[8];
        getBinaryArray(inputChar,binary);
        if(isOutputFile){
             if (inputChar == '\n'){
                 fputs("\n", outputFile);
             }else {
                for (int i = 0; i < 8; i++) {
                    fprintf(outputFile, "%d", binary[i]);
                }
                fputs(" ", outputFile);
             }
             fflush(outputFile);
        }else{
            if (inputChar == '\n'){
                printf("\n");
            }else{
                for (int i = 0; i < 8; i++) {
                   printf("%d", binary[i]); 
                }
                printf(" ");
            }
            
        }
        inputChar = fgetc(inputFile); 
    }
    fclose(outputFile);   
}

char getFixChar(char input){
    int dec = input;
    if(dec <= 'Z'){
        dec = tolower(dec);
    } 
    else {
        dec = toupper(dec);
    }
    return dec;           
}

void low2Up_up2Low(int isInputFile,FILE *inputFile,int isOutputFile, char *outputFileName){
    char inputChar = fgetc(inputFile);
    char output;
    FILE *outputFile;
    if (isOutputFile)
        outputFile = fopen(outputFileName, "w");
    while(inputChar != EOF){
        output = getFixChar(inputChar);
        if(isOutputFile){
             if (inputChar == '\n'){
                 fputs("\n", outputFile);
             }else {
                fputc(output, outputFile);
             }
             fflush(outputFile);
        }else{
            if (inputChar == '\n'){
                printf("\n");
            }else{
                printf("%c", output);
            }
            
        }
        inputChar = fgetc(inputFile);        
    }
    fclose(outputFile); 
}

FILE* getInputFile(int isInputFile, char *inputName) {
    if (isInputFile)
        return fopen(inputName, "r");
    return stdin;
}

int main(int argc, char **argv) {
    char *inputFileName;
    char *outputFileName;
    int isInputFile = 0;
    int isOutputFile = 0;
    int isB = 0;
    int isC = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (argc <= i + 1) {
                printf("Please provide input filename!");
                return 0;
            }
            inputFileName = argv[i + 1];
            isInputFile = 1;
            i++;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (argc <= i + 1) {
                printf("Please provide output filename!");
                return 0;
            }
            outputFileName = argv[i + 1];
            isOutputFile = 1;
            i++;
        } else if (strcmp(argv[i], "-c") == 0) {
            isC = 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            isB = 1;
        }
    }
    FILE *inputFile = getInputFile(isInputFile,inputFileName);

    if (isB) {
        char2binary(isInputFile, inputFile ,isOutputFile, outputFileName);
        return 0;
    } if (isC) {
        low2Up_up2Low( isInputFile,inputFile,isOutputFile,outputFileName);
        return 0;
    }
    char2ascii( isInputFile,inputFile,isOutputFile, outputFileName);
    return 0;
}