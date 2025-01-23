#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <utility>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

bool decreasingCompare(int a, int b) {
    return a > b;
}

int main(){

    std::chrono::time_point<std::chrono::system_clock> start_general = std::chrono::system_clock::now();

    int n;
    std::cin>>n;
    std::vector<int> arr(n, 0);
    for (int i = 0; i < n; i++){
        std::cin>>arr[i];
    }

    std::sort(arr.begin(), arr.end(), decreasingCompare);

    std::chrono::time_point<std::chrono::system_clock> end_general = std::chrono::system_clock::now();

    std::chrono::time_point<std::chrono::system_clock> start_greedy_algo = std::chrono::system_clock::now();

    int first, second, third;
    double max_s = 0;
    int max_third, max_second, max_first;

    for(int i = 0; i < n - 2; i++){
        first = arr[i];
        second = arr[i + 1];
        third = arr[i + 2];
        if (third + second > first && (first >= 0 && second >= 0 && third >= 0)){
            double p = double(third + second + first) / 2;
            double s = std::sqrt(p * (p - first) * (p - second) * (p - third));
            if (s > max_s){
                max_s = s;
                max_first = first;
                max_second = second;
                max_third = third;
            }
        }
    }
    std::chrono::time_point<std::chrono::system_clock> end_greedy_algo = std::chrono::system_clock::now();
    // if (n==0 || n == 1 || n == 2 || max_s == 0){
    //     std::cout<<0;
    // }
    // else{
    //     std::cout<<std::fixed<<std::setprecision(3)<<max_s<<std::endl;
    //     std::cout<<max_third<<" "<<max_second<<" "<<max_first;
    // }
    std::chrono::time_point<std::chrono::system_clock> start_native_algo = std::chrono::system_clock::now();

    double maxArea = 0;
    
    // Перебираем все возможные комбинации трех отрезков
    for (int i = 0; i < n - 2; ++i) {
        for (int j = i + 1; j < n - 1; ++j) {
            for (int k = j + 1; k < n; ++k) {
                int a = arr[i], b = arr[j], c = arr[k];
                
                // Проверяем возможность образования треугольника
                if (a + b > c && a + c > b && b + c > a) {
                    // Вычисляем полупериметр
                    double p = (double)(a + b + c) / 2;
                    
                    // Площадь по формуле Герона
                    double area = sqrt(p * (p - a) * (p - b) * (p - c));
                    
                    // Обновляем максимум
                    if (maxArea > area){
                        maxArea= area;
                    }
                }
            }
        }
    }

    std::chrono::time_point<std::chrono::system_clock> end_native_algo = std::chrono::system_clock::now();

    uint64_t algo_gready_ts = std::chrono::duration_cast<duration_t>(end_greedy_algo - start_greedy_algo + (end_general - start_general)).count();
    uint64_t native_algo_ts = std::chrono::duration_cast<duration_t>(end_native_algo - start_native_algo + (end_general - start_general)).count();

    std::cout << "Greedy algorithm time: " << algo_gready_ts << DURATION_PREFIX << std::endl;
    std::cout << "Native algorithm time: " << native_algo_ts << DURATION_PREFIX << std::endl;
 

}