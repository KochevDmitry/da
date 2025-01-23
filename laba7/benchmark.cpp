#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";


int maxRectangleInHistogram(const std::vector<int>& heights) {
    int n = heights.size();
    int maxArea = 0;

    // Для каждого прямоугольника пытаемся расширить границы
    for (int i = 0; i < n; ++i) {
        int height = heights[i];
        int left = i, right = i;

        // Расширяемся влево
        while (left > 0 && heights[left - 1] >= height) {
            --left;
        }

        // Расширяемся вправо
        while (right < n - 1 && heights[right + 1] >= height) {
            ++right;
        }

        // Вычисляем площадь текущего прямоугольника
        int width = right - left + 1;
        maxArea = std::max(maxArea, width * height);
    }

    return maxArea;
}

// Функция для вычисления максимального прямоугольника в матрице гистограмм
int maxRectangleInMatrix(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;

    int maxArea = 0;

    for (const auto& histogram : matrix) {
        // Для каждой строки-гистограммы ищем максимальный прямоугольник
        maxArea = std::max(maxArea, maxRectangleInHistogram(histogram));
    }

    return maxArea;
}

int main(){
    int m, n;
    std::string str;
    std::cin >>n >> m;
    std::vector<std::vector<int>> arr(n, std::vector<int>(m, 0));
    // std::vector<std::vector<int>> arr2(n + 1, std::vector<int>(m + 1, 0));
    std::chrono::time_point<std::chrono::system_clock> star_general = std::chrono::system_clock::now();


    for (int i = 0; i < n; i++){
        std::cin>>str;
        for (int j = 0; j < m; j++){
            if (str[j] == '0'){
                arr[i][j] = 0;
            }
            else{
                arr[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (arr[i][j] == 0){
                if (i == 0){
                    arr[i][j] = 1;
                }
                else{
                    arr[i][j] = 1 + arr[i - 1][j];
                }
            }
            else{
                arr[i][j] = 0;
            }
        }
    }

    std::chrono::time_point<std::chrono::system_clock> end_general = std::chrono::system_clock::now();

    std::chrono::time_point<std::chrono::system_clock> start_fast_algo = std::chrono::system_clock::now();

    int answer = 0;
    int max_line = 0;
    std::stack<std::pair<int, int>> gistogram;
    gistogram.push({-1, -1}); // i, h
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m + 1; j++){
            std::pair<int, int> elem = gistogram.top();

            if (j == m){
                gistogram.push({j, 0});
            }
            if (arr[i][j] > elem.second && j != m){
                gistogram.push({j, arr[i][j]});
            }
            else{
                int j_elem = arr[i][j];
                if (j == m){
                    j_elem = 0;
                }
                int test_max = 0;
                while (j_elem <= gistogram.top().second){
                    elem = gistogram.top();
                    test_max = (j - elem.first) * elem.second;
                    if (test_max > max_line){
                        max_line = test_max;
                    }
                    gistogram.pop();
                }
                gistogram.push({elem.first, arr[i][j]});
                if (answer < max_line){
                    answer = max_line;
                }
                max_line = 0;
            }
        }
        
        gistogram.pop();
        // printStack(gistogram);
    }
    std::chrono::time_point<std::chrono::system_clock> end_fast_algo = std::chrono::system_clock::now();

    std::chrono::time_point<std::chrono::system_clock> start_native_algo = std::chrono::system_clock::now();
    int result = maxRectangleInMatrix(arr);
    std::chrono::time_point<std::chrono::system_clock> end_native_algo = std::chrono::system_clock::now();

    uint64_t stack_algo_ts = std::chrono::duration_cast<duration_t>(end_fast_algo - start_fast_algo + (end_general - star_general)).count();
    uint64_t native_algo_ts = std::chrono::duration_cast<duration_t>(end_native_algo - start_native_algo + (end_general - star_general)).count();
 
    std::cout << "Stack algorithm time: " << stack_algo_ts << DURATION_PREFIX << std::endl;
    std::cout << "Native algorithm time: " << native_algo_ts << DURATION_PREFIX << std::endl;

}