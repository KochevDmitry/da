#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>

bool decreasingCompare(int a, int b) {
    return a > b;
}

int main(){
    int n;
    std::cin>>n;
    std::vector<int> arr(n, 0);
    for (int i = 0; i < n; i++){
        std::cin>>arr[i];
    }

    std::sort(arr.begin(), arr.end(), decreasingCompare);


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
    if (n==0 || n == 1 || n == 2 || max_s == 0){
        std::cout<<0;
    }
    else{
        std::cout<<std::fixed<<std::setprecision(3)<<max_s<<std::endl;
        std::cout<<max_third<<" "<<max_second<<" "<<max_first;
    }

}