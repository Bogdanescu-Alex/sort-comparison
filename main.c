#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "database.h"

// used void* so it works for any type
typedef void* Pointer;
typedef int (*CompareFunction)(const void*, const void*);

// just swaps two things in memory
void swap_elements(Pointer a, Pointer b, int size) {
    void* temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

// bubble sort
void myBubbleSort(Pointer base, int n, int size, CompareFunction cmp) {
    char* data = (char*)base;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (cmp(data + j * size, data + (j + 1) * size) > 0) {
                swap_elements(data + j * size, data + (j + 1) * size, size);
            }
        }
    }
}

// selection sort
void mySelectionSort(Pointer base, int n, int size, CompareFunction cmp) {
    char* data = (char*)base;
    for (int i = 0; i < n - 1; i++) {
        int small = i;
        for (int j = i + 1; j < n; j++) {
            if (cmp(data + j * size, data + small * size) < 0) {
                small = j;
            }
        }
        swap_elements(data + i * size, data + small * size, size);
    }
}

// insertion sort
void myInsertionSort(Pointer base, int n, int size, CompareFunction cmp) {
    char* data = (char*)base;
    void* key = malloc(size);
    for (int i = 1; i < n; i++) {
        memcpy(key, data + i * size, size);
        int j = i - 1;
        while (j >= 0 && cmp(data + j * size, key) > 0) {
            memcpy(data + (j + 1) * size, data + j * size, size);
            j--;
        }
        memcpy(data + (j + 1) * size, key, size);
    }
    free(key);
}

// shell sort
void myShellSort(Pointer base, int n, int size, CompareFunction cmp) {
    char* data = (char*)base;
    void* temp = malloc(size);
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            memcpy(temp, data + i * size, size);
            int j;
            for (j = i; j >= gap && cmp(data + (j - gap) * size, temp) > 0; j -= gap) {
                memcpy(data + j * size, data + (j - gap) * size, size);
            }
            memcpy(data + j * size, temp, size);
        }
    }
    free(temp);
}

// quick sort helper to split array
int partition(char* data, int low, int high, int size, CompareFunction cmp) {
    void* pivot = data + high * size;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (cmp(data + j * size, pivot) < 0) {
            i++;
            swap_elements(data + i * size, data + j * size, size);
        }
    }
    swap_elements(data + (i + 1) * size, data + high * size, size);
    return i + 1;
}

// the recursive part of quick sort
void quickSortRecursive(char* data, int low, int high, int size, CompareFunction cmp) {
    if (low < high) {
        int p = partition(data, low, high, size, cmp);
        quickSortRecursive(data, low, p - 1, size, cmp);
        quickSortRecursive(data, p + 1, high, size, cmp);
    }
}

// quick sort
void myQuickSort(Pointer base, int n, int size, CompareFunction cmp) {
    quickSortRecursive((char*)base, 0, n - 1, size, cmp);
}

// radix sort helper
int getMax(int* arr, int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx) mx = arr[i];
    return mx;
}

// radix sort helper
void countSort(int* arr, int n, int exp) {
    int* output = malloc(n * sizeof(int));
    int i, count[10] = {0};

    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (i = 0; i < n; i++)
        arr[i] = output[i];
    
    free(output);
}

// radix sort
void myRadixSort(Pointer base, int n, int size, CompareFunction cmp) {
    if (size != sizeof(int)) return;

    int* arr = (int*)base;
    int m = getMax(arr, n);

    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

int compareInts(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int compareStrings(const void* a, const void* b) {
    return strcmp(*(char**)a, *(char**)b);
}

void fillRandom(int* a, int n) {
    for (int i = 0; i < n; i++) a[i] = rand() % 1000;
}

void fillNearlySorted(int* a, int n) {
    for (int i = 0; i < n; i++) a[i] = i;
    for (int i = 0; i < 10; i++) {
        int r1 = rand() % n;
        int r2 = rand() % n;
        int t = a[r1]; a[r1] = a[r2]; a[r2] = t;
    }
}

// runs the sort and saves result to db
void runTest(const char* dataset_name, const char* algo_name, void (*sortFunc)(Pointer, int, int, CompareFunction), void* data, int n, int size, CompareFunction cmp) {
    void* copy = malloc(n * size);
    memcpy(copy, data, n * size);
    
    clock_t start = clock();
    sortFunc(copy, n, size, cmp);
    clock_t end = clock();
    
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%-20s took %f seconds\n", algo_name, time_taken);
    
    save_to_db(dataset_name, algo_name, n, time_taken);
    
    free(copy);
}

int main() {
    srand(time(NULL));
    int n = 2000;
    
    printf("--- start sort comparison ---\n");
    printf("array size: %d\n\n", n);

    int* numbers = malloc(n * sizeof(int));
    
    // random number tests
    fillRandom(numbers, n);
    printf("testing: random numbers\n");
    runTest("Random Numbers", "Bubble Sort", myBubbleSort, numbers, n, sizeof(int), compareInts);
    runTest("Random Numbers", "Selection Sort", mySelectionSort, numbers, n, sizeof(int), compareInts);
    runTest("Random Numbers", "Insertion Sort", myInsertionSort, numbers, n, sizeof(int), compareInts);
    runTest("Random Numbers", "Shell Sort", myShellSort, numbers, n, sizeof(int), compareInts);
    runTest("Random Numbers", "Quick Sort", myQuickSort, numbers, n, sizeof(int), compareInts);
    runTest("Random Numbers", "Radix Sort", myRadixSort, numbers, n, sizeof(int), compareInts);
    printf("\n");

    // nearly sorted tests
    fillNearlySorted(numbers, n);
    printf("testing: nearly sorted\n");
    runTest("Nearly Sorted", "Bubble Sort", myBubbleSort, numbers, n, sizeof(int), compareInts);
    runTest("Nearly Sorted", "Selection Sort", mySelectionSort, numbers, n, sizeof(int), compareInts);
    runTest("Nearly Sorted", "Insertion Sort", myInsertionSort, numbers, n, sizeof(int), compareInts);
    runTest("Nearly Sorted", "Shell Sort", myShellSort, numbers, n, sizeof(int), compareInts);
    runTest("Nearly Sorted", "Quick Sort", myQuickSort, numbers, n, sizeof(int), compareInts);
    runTest("Nearly Sorted", "Radix Sort", myRadixSort, numbers, n, sizeof(int), compareInts);
    printf("\n");

    // string tests
    printf("testing: random strings\n");
    char** words = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        words[i] = malloc(10);
        sprintf(words[i], "str%d", rand() % 1000);
    }
    runTest("Random Strings", "Bubble Sort", myBubbleSort, words, n, sizeof(char*), compareStrings);
    runTest("Random Strings", "Selection Sort", mySelectionSort, words, n, sizeof(char*), compareStrings);
    runTest("Random Strings", "Insertion Sort", myInsertionSort, words, n, sizeof(char*), compareStrings);
    runTest("Random Strings", "Shell Sort", myShellSort, words, n, sizeof(char*), compareStrings);
    runTest("Random Strings", "Quick Sort", myQuickSort, words, n, sizeof(char*), compareStrings);
    //radix only on numbers
    
    for (int i = 0; i < n; i++) free(words[i]);
    free(words);
    free(numbers);

    printf("\ndone!\n");
    return 0;
}
