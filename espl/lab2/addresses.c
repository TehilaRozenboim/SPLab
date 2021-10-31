#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5; //uninitialized data bss address
int addr6; //uninitialized data bss address

////text segment->function declaration
int foo();
void point_at(void *p);
void task1c();

int main (int argc, char** argv){
    int addr2; //stack
    int addr3; //stack
    char* yos="ree";  //yos pointer in stack-> text segment ??
    int * addr4 = (int*)(malloc(50)); //addr4 pointer in stack -> heap
    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);
    
	point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos);
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);

    task1c(); 

    return 0;
}
//text segment -> func declaration
int foo(){
    return -1;
}
//text segment
void point_at(void *p){
    int local; //stack
	static int addr0 = 2; //initialized data
    static int addr1; //uninitialized data BSS


    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}

void task1c() {
	int iarray[3];
	char carray[3];
    
    for (int i = 0; i < 3; i++) {
		printf("- The value at index %d in iarray is %p\n", i, &iarray[i]);
	}
	for (int i = 0; i < 3; i++) {
		printf("- The value at index %d in carray is %p\n", i, &carray[i]);
	}
	printf("- iarray: %p\n", iarray);
	printf("- iarray+1: %p\n", iarray + 1);
	printf("- carray: %p\n", carray);
	printf("- carray+1: %p\n", carray + 1);
}


