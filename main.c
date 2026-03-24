#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void insertSort(long *arr, long len) {
    for (long i = 1; i < len; ++i) {
        long key = arr[i];
        long j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void bubbleSort(long *arr, long len) {
    for (long i = 0; i < len - 1; i++) {
        for (long j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                long temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selectionSort(long *arr, long len) {
    for (long i = 0; i < len - 1; i++) {
        long min_idx = i;
        for (long j = i + 1; j < len; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        long temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

void merge(long *arr, long l, long m, long r) {
    long n1 = m - l + 1;
    long n2 = r - m;
    long *L = (long *)malloc(n1 * sizeof(long));
    long *R = (long *)malloc(n2 * sizeof(long));

    for (long i = 0; i < n1; i++) L[i] = arr[l + i];
    for (long j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    long i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    free(L); free(R);
}
void mergeSort(long *arr, long l, long r) {
    if (l < r) {
        long m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
void mergeSortWrapper(long *arr, long len) {
    mergeSort(arr, 0, len - 1);
}

long quickSortPartition(long *arr, long low, long high) { // lomuto partition scheme    
    long pivot = arr[high]; 
    long i = (low - 1);
    for (long j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            long temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    long temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quickSort(long *arr, long low, long high) {
    if (low < high) {
        long pi = quickSortPartition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void quickSortWrapper(long *arr, long len) {
    quickSort(arr, 0, len - 1);
}




void generateArray(long *arr, long len) {
    for (long i = 0; i < len; i++) {
        arr[i] = rand() % 1000;
    }
}

void printArray(long *arr, long len) {
    printf("\n [ ");
    for (long i = 0; i < len; i++) {
        printf("%ld%s", arr[i], ", ");
    }
    printf(" ]\n");
}

double monteCarlo(long totalNrOfSimulations, void (*sortFunc)(long *, long), long *arr, long len) {
    double totalTime = 0;
    
    for (long i = 0; i < totalNrOfSimulations; i++) {
        long *arrCopy = (long *)malloc(len * sizeof(long));
        if (!arrCopy) return -1.0;
        
        memcpy(arrCopy, arr, len * sizeof(long));

        clock_t start = clock();
        sortFunc(arrCopy, len);
        clock_t end = clock();

        totalTime += (double)(end - start) / CLOCKS_PER_SEC;
        free(arrCopy);
    }
    return totalTime / (double)totalNrOfSimulations;
}

int main() {
    srand((unsigned int)time(NULL));
    
    long n = 100000; 
    long simulations = 5;
    long *arrayStart = (long *)malloc(n * sizeof(long));
    
    if (!arrayStart) {
        return 1;
    }

    generateArray(arrayStart, n);
    
    double avgSeconds = monteCarlo(simulations, bubbleSort, arrayStart, n);

    // printArray(arrayStart, n);
    printf("Average Time over %ld simulations: %f seconds\n", simulations, avgSeconds);


    free(arrayStart);
    return 0;
}