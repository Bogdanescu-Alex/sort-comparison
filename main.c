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
long long monteCarlo(short totalNrOfSimulations, void (*sortFunc)(short *, short), short *arr, short len) {
    unsigned long long averageTime = 0;
    unsigned int dummy;
    for (short i = 0; i < totalNrOfSimulations; i++) {
        short *arrCopy = (short *)malloc(len * sizeof(short));
        for (short j = 0; j < len; j++) {
            *(arrCopy + j) = *(arr + j);
        }
        unsigned long long start = __rdtscp(&dummy);
        sortFunc(arrCopy, len);
        unsigned long long end = __rdtscp(&dummy);
        long sortTime = end - start;
        averageTime += sortTime;
        free(arrCopy);
    }
    return averageTime / totalNrOfSimulations;        
}

int main() {
    short n = 10000;
    short *arrayStart = (short *) malloc(n * sizeof(short));
    generateArray(arrayStart, n);
    
    printf("TOOK %ld CPU CYCLES", monteCarlo(500, insertSort, arrayStart, n));
    // printArray(arrayStart, n);
    free(arrayStart);
    return 0;
}