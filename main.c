#include<stdlib.h>
#include<stdio.h>
#if defined(_MSC_VER)
# include <intrin.h>
#else
# include <x86intrin.h>
#endif

void insertSort(short *arr, short len) {
    for (short i = 1; i < len; ++i) {
        short key = *(arr + i);
        short j = i - 1;

        while (j >= 0 && *(arr + j) > key) {
            *(arr + j + 1) = *(arr + j);
            j = j - 1;
        }
        *(arr + j + 1) = key;
    }
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
    
    unsigned int dummy;
    unsigned long long start = __rdtscp(&dummy);

    insertSort(arrayStart, n);
    
    unsigned long long end = __rdtscp(&dummy);
    long sortTime = end - start;

    printf("TOOK %li CPU CYCLES", sortTime);
    // printArray(arrayStart, n);
    free(arrayStart);
    return 0;
}