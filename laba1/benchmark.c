#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


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

int compare_by_first(const void *a, const void *b) {
    const unsigned long long *elem_a = (const unsigned long long *)a;
    const unsigned long long *elem_b = (const unsigned long long *)b;

    return (int)(elem_a[0] - elem_b[0]);
}

int compare_by_second(const void *a, const void *b) {
    const unsigned long long *elem_a = (const unsigned long long *)a;
    const unsigned long long *elem_b = (const unsigned long long *)b;

    return (int)(elem_a[1] - elem_b[1]);
}

int compare_by_third(const void *a, const void *b) {
    const unsigned long long *elem_a = (const unsigned long long *)a;
    const unsigned long long *elem_b = (const unsigned long long *)b;

    return (int)(elem_a[2] - elem_b[2]);
}

int main() {
    long long n = 1, size = 0, data1, data2, data3;
    n = 1000000;
    unsigned long long val;
    int max_d = 31, max_m = 12, max_y = 9999;
    char input[100];
    unsigned long long (*arr)[6] = malloc(n * sizeof(unsigned long long[6]));
    long long i = 0;

    while(fgets(input, sizeof(input), stdin) != NULL){
        if (i == n){
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
        arr[i][0] = data1;
        arr[i][1] = data2;
        arr[i][2] = data3;
        arr[i][3] = val;
        i+=1;
        size += 1;
        memset(input, 0, sizeof(input));
    }

    unsigned long long (*array)[6] = malloc(size * sizeof(unsigned long long[6]));

    for (long long i = 0; i < size; i++){
        array[i][0] = arr[i][0];
        array[i][1] = arr[i][1];
        array[i][2] = arr[i][2];
        array[i][3] = arr[i][3];
        array[i][4] = arr[i][4];
        array[i][5] = arr[i][5];
    }

    printf("Count of lines is %lld\n", size);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); // начальное время

    // Измеряем время работы сортировки подсчётом.
    
    counting_sort(arr, max_d, size, 0);
    counting_sort(arr, max_m, size, 1);
    counting_sort(arr, max_y, size, 2);

    gettimeofday(&end_time, NULL); // конечное время
    long long start_ms = start_time.tv_sec * 1000LL + start_time.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    long long end_ms = end_time.tv_sec * 1000LL + end_time.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    double search_time_radix = (end_ms - start_ms) / 1000.0; // искомо
    

    // Измеряем время работы stl сортировки.

    struct timeval start_time_2, end_time_2;
    gettimeofday(&start_time_2, NULL); // начальное время
    qsort(array, size, sizeof(unsigned long long[6]), compare_by_first);
    qsort(array, size, sizeof(unsigned long long[6]), compare_by_second);
    qsort(array, size, sizeof(unsigned long long[6]), compare_by_third);

    gettimeofday(&end_time_2, NULL); // конечное время
    long long start_ms_2 = start_time_2.tv_sec * 1000LL + start_time_2.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    long long end_ms_2 = end_time_2.tv_sec * 1000LL + end_time_2.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    double search_time_qsort = (end_ms_2 - start_ms_2) / 1000.0; // искомо

    for (long long i = 0; i < size; i++){
        if (array[i][4] == 1)
            printf("0");
        printf("%lld.", array[i][0]);
        if (array[i][5] == 1)
            printf("0");
        printf("%lld.", array[i][1]);
        printf("%lld\t%llu\n", array[i][2], array[i][3]);
    }
    
    printf("Radix sort time: %.4f\n", search_time_radix);
    printf("Qsort time: %.4f\n", search_time_qsort);

    free(arr);
    free(array);

    return 0;
}