#include <iostream>
#include <vector>
#include <stack>
#include <utility>

// template <typename T>
// void printStack(std::stack<T> s) {
//     // Проходим через копию стека, чтобы исходный не изменился
//     while (!s.empty()) {
//         std::cout <<"("<< s.top().first <<", "<< s.top().first << ") ";
//         s.pop(); // Удаляем верхний элемент из копии
//     }
//     std::cout << std::endl;
// }


int main(){
    int m, n;
    std::string str;
    std::cin >>n >> m;
    std::vector<std::vector<int>> arr(n, std::vector<int>(m, 0));
    // std::vector<std::vector<int>> arr2(n + 1, std::vector<int>(m + 1, 0));

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
    // std::cout<<std::endl;
    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < m; j++){
    //         std::cout<<arr[i][j];
    //     }
    //     std::cout<<std::endl;
    // }
    // std::cout<<std::endl;

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
    std::cout<<answer;

}