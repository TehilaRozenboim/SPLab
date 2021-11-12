#include <stdio.h>


int get_diff(char a, char b) {
    int dec_a = a;
    int dec_b = b;
    return dec_a-dec_b;
}

int get_len(char*str) {
    int len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

int cmpstr(char* str1, char* str2) {
    int str1_len = get_len(str1);
    int str2_len = get_len(str2);

    int size;
    if (str1_len > str2_len) {
        size = str1_len;
    } else {
        size = str2_len;
    }
    int diff = 0;

    for (int i = 0; i < size; i++) {
        if (str1[i] != str2[i]) {
            diff = get__diff(str1[i], str2[i]);
            return diff;
        }
    }

    return diff;
}