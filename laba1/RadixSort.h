#ifndef SORTING_H
#define SORTING_H

#include <stdlib.h>

void radix_sort(unsigned long long (*arr)[6], long long max_d, long long max_m, long long max_y, long long size);
void counting_sort(unsigned long long (*arr)[6], long long max_val, long long size, long long key);

#endif // SORTING_H