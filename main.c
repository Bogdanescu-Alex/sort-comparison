#include<stdlib.h>
#include<stdio.h>
#include<time.h>

void sortFunc() {

}

void generateArray(short *arr, short len) {
    for (short i = 0; i < len; i++) {
        *(arr + i) = rand() % 1000; // we % 1000 to have a int [0, 999], since rand can return max int 
    }
}

void printArray(short *arr, short len) {
    printf("\n [ ");
    for (short i = 0; i < len; i++) {
        printf("%hd, ", *(arr + i));
    }
    printf("]\n"); 
}


int main() {
    short n = 10000;
    short *arrayStart = (short *) malloc(n * sizeof(short));
    generateArray(arrayStart, n);
    printArray(arrayStart, n);
    free(arrayStart);
    return 0;
}