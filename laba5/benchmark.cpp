#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <exception>
#include <utility>
#include <algorithm>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

struct Triple {
    int str;
    int index;
    int eq;
};

bool compareByStr(const Triple &a, const Triple &b){
    return a.str < b.str;
}
bool compareByEq(const Triple &a, const Triple &b){
    return a.eq < b.eq;
}

void naiveSearch(const std::string &text, const std::string &pattern, int &patterncount) {
    int textLength = text.size();
    int patternLength = pattern.size();
    bool found = false;

    // Проход по тексту с поиском подстроки
    for (int i = 0; i <= textLength - patternLength; ++i) {
        int j = 0;
        // Проверяем, совпадает ли подстрока с паттерном
        for (j = 0; j < patternLength; ++j) {
            if (text[i + j] != pattern[j]) {
                break;  // Прекращаем, если символы не совпадают
            }
        }

        // Если подстрока совпала
        if (j == patternLength) {
            found = true;
            patterncount++;
        }
    }

    // Если паттерн не найден
    if (!found) {
    }


}

int main(){

    int patternCountsuff = 0;

    std::ifstream inputFile1("test1.txt");
    if (!inputFile1.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл." << std::endl;
        return 1;
    }

    std::string text;
    if (!std::getline(inputFile1, text)) {
        std::cerr << "Ошибка: файл пуст или не удалось прочитать текст." << std::endl;
        return 1;
    }
    int len_text = text.size();
    int global_len_text = text.size();
    int count_sentinels = pow(2, int(log2(len_text)) + 1) - len_text; // количество сентинелов до степени двойки 
    for (int i = 0; i < count_sentinels; i ++){
        text.push_back('$');
    }
    len_text = text.size();

    std::vector<Triple> big_arr;
    for (int i = 0; i < len_text; i++){
        Triple t;
        t.str = int(text[i]);
        t.index = i;
        t.eq = 0;
        big_arr.push_back(t);
    }


    std::stable_sort(big_arr.begin(), big_arr.end(), compareByStr);

    std::chrono::time_point<std::chrono::system_clock> start_suff = std::chrono::system_clock::now();

    big_arr[0].eq = 1;  
    int dop_eq[len_text];
    dop_eq[big_arr[0].index] = 1;
    for (int i = 1; i < len_text; i++){
        if (big_arr[i].str != big_arr[i - 1].str){
            big_arr[i].eq = big_arr[i - 1].eq + 1;
        }
        else big_arr[i].eq = big_arr[i - 1].eq;
        dop_eq[big_arr[i].index] = big_arr[i].eq;
    }

    // std::cout<<text<<std::endl;
    // char a = '$';
    // char b = '$';
    // std::cout<<int('$')<<" "<<int('A')<<std::endl;
    // for (int i = 0; i < len_text; i ++){
    //     std::cout<<char(big_arr[i].str)<<" "<<big_arr[i].index<<" "<<big_arr[i].eq<<std::endl;
    // }
    // for (int i = 0; i < len_text; i ++){
    //     std::cout<<dop_eq[i]<<" ";
    // }
    // std::cout<<std::endl;

    int count_symbol = 1;
    while (big_arr.back().eq != len_text){
        for (int i = 0; i < len_text; i++){ 
            big_arr[i].index -= count_symbol;
            if (big_arr[i].index < 0)
                big_arr[i].index += len_text;
            big_arr[i].eq = dop_eq[big_arr[i].index];
        }

        std::stable_sort(big_arr.begin(), big_arr.end(), compareByEq);

        std::vector<std::pair<int, int>> pair_eq;

        for (int i = 0; i < len_text; i++){
            int ind = big_arr[i].index + count_symbol;
            if (ind >= len_text)
                ind -= len_text;
            pair_eq.push_back(std::make_pair(big_arr[i].eq, dop_eq[ind]));
        }
        
        big_arr[0].eq = 1;
        dop_eq[big_arr[0].index] = 1;
        for (int i = 1; i < len_text; i++){
            if (pair_eq[i] != pair_eq[i - 1]){
                big_arr[i].eq = big_arr[i - 1].eq + 1;
            }
            else big_arr[i].eq = big_arr[i - 1].eq;
            dop_eq[big_arr[i].index] = big_arr[i].eq;
        }
        count_symbol *= 2;
    }
    std::vector<int> suff_array;
    for (int i = count_sentinels; i < len_text; i ++){
        // std::cout<<big_arr[i].index<<" ";
        suff_array.push_back(big_arr[i].index);
    }
    // std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////
    int indexes = 1;
    std::string pattern;
    while (std::getline(inputFile1, pattern)) {
        std::vector<int> answer;
        int index_lower = -1;
        int index_upper = -1;
        int L = 0;
        int R = global_len_text - 1;
        int M = (L + R) / 2;
        while (R >= L){
            M = (L + R) / 2;
            for (int i = 0; i <= pattern.size(); i ++){
                if (i == pattern.size()){
                    index_lower = M;
                    R = M - 1;
                    break;
                }
                if (suff_array[M] + i == global_len_text){
                    L = M + 1;
                    break;
                }
                if (pattern[i] == text[suff_array[M] + i]){
                    continue;
                }
                else if(pattern[i] > text[suff_array[M] + i]){
                    L = M + 1;
                    break;
                }
                else {
                    R = M - 1;
                    break;
                }
            }
        } 
        L = 0;
        R = global_len_text - 1;
        while (R >= L){
            M = (L + R) / 2;
            for (int i = 0; i <= pattern.size(); i ++){
                if (i == pattern.size()){
                    index_upper = M;
                    L = M + 1;
                    break;
                }
                if (suff_array[M] + i == global_len_text){
                    L = M + 1;
                    break;
                }
                if (pattern[i] == text[suff_array[M] + i]){
                    continue;
                }
                else if(pattern[i] > text[suff_array[M] + i]){
                    L = M + 1;
                    break;
                }
                else {
                    R = M - 1;
                    break;
                }
            }
        } 
        if (index_lower != -1 && index_upper != -1){
            patternCountsuff += (index_upper - index_lower + 1);
        }
        indexes += 1;
        
    }

    auto end_suff = std::chrono::system_clock::now();
    uint64_t suff_array_ts = std::chrono::duration_cast<duration_t>( end_suff - start_suff ).count();

    inputFile1.close();

    std::ifstream inputFile2("test1.txt");
    if (!inputFile2.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл." << std::endl;
        return 1;
    }

    // Чтение первой строки - текста
    if (!std::getline(inputFile2, text)) {
        std::cerr << "Ошибка: файл пуст или не удалось прочитать текст." << std::endl;
        return 1;
    }

    int patterncount = 0;
    std::chrono::time_point<std::chrono::system_clock> start_naive = std::chrono::system_clock::now();


    // Чтение паттернов построчно и их поиск в тексте
    while (std::getline(inputFile2, pattern)) {
        naiveSearch(text, pattern, patterncount);
    }

    auto end_naive = std::chrono::system_clock::now();
    uint64_t naive_ts = std::chrono::duration_cast<duration_t>( end_naive - start_naive ).count();

    inputFile2.close();
    std::cout << "Suffix array algorithm time: " << suff_array_ts << DURATION_PREFIX << std::endl;
    std::cout << "Native algorithm time: " << naive_ts << DURATION_PREFIX << std::endl;
    std::cout<<"Suffix array algorithm: "<<patternCountsuff<<" Native algorithm: "<<patterncount<< std::endl;
}