#include <stdio.h>
#include <stdlib.h>



void counting_sort(unsigned long long (*arr)[6], long long max_val, long long size, long long key){
    
    long long list[max_val + 1];

    for (long long i = 0; i <= max_val; i++) {
        list[i] = 0;
    }

    for (long long i = 0; i < size; i++){
        list[arr[i][key]] += 1;
    }

    for (long long i = 1; i <= max_val; i++){
        list[i] += list[i - 1];
    }

    unsigned long long (*arr2)[6] = malloc(size * sizeof(unsigned long long[6]));
    for (long long i = size - 1; i >= 0; i--){
        arr2[list[arr[i][key]] - 1][0] = arr[i][0];
        arr2[list[arr[i][key]] - 1][1] = arr[i][1]; 
        arr2[list[arr[i][key]] - 1][2] = arr[i][2];
        arr2[list[arr[i][key]] - 1][3] = arr[i][3];
        arr2[list[arr[i][key]] - 1][4] = arr[i][4];
        arr2[list[arr[i][key]] - 1][5] = arr[i][5];
        list[arr[i][key]] -= 1;
    }

    for (long long i = 0; i < size; i++){
        arr[i][0] = arr2[i][0];
        arr[i][1] = arr2[i][1];
        arr[i][2] = arr2[i][2];
        arr[i][3] = arr2[i][3];
        arr[i][4] = arr2[i][4];
        arr[i][5] = arr2[i][5];
    }
    
    free(arr2);

}

void radix_sort(unsigned long long (*arr)[6], long long max_d, long long max_m, long long max_y, long long size){
    counting_sort(arr, max_d, size, 0);
    counting_sort(arr, max_m, size, 1);
    counting_sort(arr, max_y, size, 2);
}