// c++ -O2 -o benchmark benchmark.cpp //оптимизация программы, можно -O3
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

int counter = 0;

void dfs(std::vector<std::vector<int>>& matrix, std::vector<int>& used, int node, std::vector<int>& answer){
    answer.push_back(node + 1);
    used[node] = 1;
    while (used[counter] == 1){
        counter++;
    }
    for (int i = 0; i < matrix[node].size(); i++){
        if (used[matrix[node][i]] == 0){
            dfs(matrix, used, matrix[node][i], answer);
        }
    }
}

int main(){
    int n, m;
    std::cin>>n>>m;
    std::vector<std::vector<int>> matrix(n);

    std::chrono::time_point<std::chrono::system_clock> start_algo = std::chrono::system_clock::now();

    for (int i = 0; i < m; i++){
        int a, b;
        std::cin>>a>>b;
        matrix[a - 1].push_back(b - 1);
        matrix[b - 1].push_back(a - 1);
    }
    
    std::vector<int> used(n);
    while (counter != n){
        std::vector<int> answer;
        dfs(matrix, used, counter, answer);
        sort(answer.begin(), answer.end());
        for (int i = 0; i < answer.size(); i++){
            // std::cout<<answer[i]<<" ";
        }

        // std::cout<<std::endl;
    }
    std::chrono::time_point<std::chrono::system_clock> end_algo = std::chrono::system_clock::now();
    uint64_t algo_ts = std::chrono::duration_cast<duration_t>(end_algo - start_algo).count();

    std::cout << "Algorithm time: " << algo_ts << DURATION_PREFIX << std::endl;

}
