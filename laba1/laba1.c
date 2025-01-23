#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RadixSort.h"


int main(){
    long long n = 1, size = 0, data1, data2, data3;
    n = 1000000;
    unsigned long long val;
    // long long max_d = 0, max_m = 0, max_y = 0;  
    int max_d = 31, max_m = 12, max_y = 9999;  
    char point;
    char input[100];
    
    unsigned long long (*arr)[6] = malloc(n * sizeof(unsigned long long[6]));
    long long i = 0;

    while(fgets(input, sizeof(input), stdin) != NULL){ // ввод значений

        if (i == n){ // если привысили
            arr = realloc(arr, 2 * n * sizeof(unsigned long long[6]));
            n *= 2;
        }

        if (input[0] == '\n'){
            continue;
        }

        arr[i][4] = 0;
        arr[i][5] = 0;

        if (input[0] == '0'){
            arr[i][4] = 1;
            if (input[3] == '0'){
                arr[i][5] = 1;
            }
        }
        else if (input[2] == '.' && input[3] == '0'){
            arr[i][5] = 1;
        }
        else if (input[1] == '.' && input[2] == '0'){
            arr[i][5] = 1;
        }

        sscanf(input, "%lld.%lld.%lld\t%llu", &data1, &data2, &data3, &val);
        // printf("\ncheck: data1: %lld, data2: %lld, data3: %lld, val: %llu\n", data1, data2, data3, val);

        arr[i][0] = data1;
        arr[i][1] = data2;
        arr[i][2] = data3;
        arr[i][3] = val;

        // printf("check in ARRAY: data1: %lld, data2: %lld, data3: %lld, val: %llu\n", arr[i][0], arr[i][1], arr[i][2], arr[i][3]);

        i+=1;
        size += 1;

        memset(input, 0, sizeof(input));
    }

    // counting_sort(arr, max_d, size, 0);
    // counting_sort(arr, max_m, size, 1);
    // counting_sort(arr, max_y, size, 2);
    radix_sort(arr, max_d, max_m, max_y, size);



    for (long long i = 0; i < size; i++){
        if (arr[i][4] == 1)
            printf("0");
        printf("%lld.", arr[i][0]);
        if (arr[i][5] == 1)
            printf("0");
        printf("%lld.", arr[i][1]);
        printf("%lld\t%llu\n", arr[i][2], arr[i][3]);
    }


    free(arr);

}

// if (input[0] == '0'){
//             if (input[3] == '0'){
//                 data1 = stoi(input.substr(0, 2));
//                 data2 = stoi(input.substr(3, 2));
//                 data3 = stoi(input.substr(6, 4));
//                 val = stoi(input.substr(11));
//                 arr[i][4] = 1; arr[i][5] = 1;
//             }
//             else{
//                 data1 = stoi(input.substr(0, 2));
//                 data2 = stoi(input.substr(3, 2));
//                 data3 = stoi(input.substr(6, 4));
//                 val = stoi(input.substr(11));
//                 arr[i][4] = 1; arr[i][5] = 1;
//             }
//         }
