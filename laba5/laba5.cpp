#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <exception>
#include <utility>
#include <algorithm>

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

int main(){
    std::string text;
    std::cin >> text;
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
    while (std::cin >> pattern) {
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
        // std::cout<<index_lower<<" "<<index_upper<<std::endl;
        if (index_lower != -1 && index_upper != -1){

            for (int i = index_lower; i <= index_upper; i++){
                answer.push_back(suff_array[i]);
            }
            std::sort(answer.begin(), answer.end());
            std::cout<<indexes<<": ";
            for (int i = 0; i < answer.size(); i++){
                if (i == answer.size() - 1){
                    std::cout<<answer[i] + 1<<std::endl;
                }
                else std::cout<<answer[i] + 1<<", ";
            }
        }
        indexes += 1;
    }

    
}