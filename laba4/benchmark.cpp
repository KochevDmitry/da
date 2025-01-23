#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

/*
using duration_t = std::chrono::milliseconds;
const std::string DURATION_PREFIX = "ms";
*/


std::vector<int> naive_search(const std::vector<std::string>& pattern, const std::vector<std::string>& text) {
    std::vector<int> result;
    int pattern_size = pattern.size();
    int text_size = text.size();

    // Проверка на пустые вектора
    if (pattern_size == 0 || text_size == 0 || pattern_size > text_size) {
        return result; // возвращаем пустой результат
    }

    // Наивный алгоритм поиска подстроки (слово в слово)
    for (int i = 0; i <= text_size - pattern_size; ++i) {
        bool found = true;
        for (int j = 0; j < pattern_size; ++j) {
            if (text[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            result.push_back(i);
        }
    }

    return result;
}


std::map<std::string, std::vector<int>> letter_indices;

std::vector<std::pair<int, int>> processStrings(const std::vector<std::string>& string_vector) {
    std::vector<std::pair<int, int>> wordstring;
    
    for (size_t i = 0; i < string_vector.size(); ++i) {
        std::istringstream stream(string_vector[i]);
        std::string word;
        int word_count = 0;

        while (stream >> word) {
            ++word_count;
            wordstring.emplace_back(i + 1, word_count); 
        }
    }
    
    return wordstring;
}

// Функция для разворота строки
void reverseString(std::string& s) {
    int n = s.length();
    for (int i = 0; i < n / 2; ++i) {
        std::swap(s[i], s[n - i - 1]);
    }
}

// Функция для разворота вектора
template <typename T>
void reverseVector(std::vector<T>& v) {
    int n = v.size();
    for (int i = 0; i < n / 2; ++i) {
        std::swap(v[i], v[n - i - 1]);
    }
}

std::vector<int> zFunction(const std::vector<std::string>&  s){
    int n = s.size();
    std::vector<int> z(n, 0);
    int l = 0, r = 0;
    for (int k = 1; k < n; k++){
        if (r < k){
            int i = 0;
            int count = 0;
            while (k + i < n && s[i] == s[k + i]){
                count += 1;
                i += 1;
            }
            z[k] = count;
            l = k;
            r = l + z[l] - 1;
        }
        else{
            if (z[k - l] < r - k + 1){
                z[k] = z[k - l];
            }
            else{
                int i = r - k + 1;
                int count = r - k + 1;
                while (k + i < n && s[i] == s[k + i]){
                    count += 1;
                    i += 1;
                }
                z[k] = count;
                l = k;
                r = l + z[l] - 1;
            }
        }
    }
    return z;
}

int UPPS(std::string letter, int index){
    int i = 0;
    int shift = 0;
    if (letter_indices.count(letter) > 0){
        while(letter_indices[letter][i] < index && letter_indices[letter].size() > i){ 
            i += 1;
        }
        i -= 1;
        shift = index - letter_indices[letter][i];
    }
    return shift;
}


int main() {
    std::string pattern_str;
    std::vector<std::string> string_vector;
    std::string input;
    std::vector<int> answer;

    std::getline(std::cin, pattern_str);

    for (char& c : pattern_str) {
        c = std::tolower(c);
    }


    std::vector<std::string> pattern;
    std::stringstream ss(pattern_str);
    std::string word;

    while (ss >> word) {
        pattern.push_back(word);
    }

    while (std::getline(std::cin, input)) {
        string_vector.push_back(input); 
    }

    std::vector<std::pair<int,int>> wordstring = processStrings(string_vector); 
    // std::vector<std::pair<int,int>> wordstringNative = processStrings(string_vector); 

    std::string text_str;
    for (const auto& str : string_vector) {
        if(!text_str.empty()){
            text_str += " ";
        }
        text_str += str;
    }

    for (char& c : text_str) {
        c = std::tolower(c);
    }

    std::vector<std::string> text;
    std::stringstream tt(text_str);
    std::string word_text;

    while (tt >> word) {
        text.push_back(word);
    }

    int pattern_length = pattern.size();

    std::chrono::time_point<std::chrono::system_clock> start_AG = std::chrono::system_clock::now();

    // Заполнение словаря
    for (int i = 0; i < pattern_length; ++i) {
        std::string letter = pattern[i];
        letter_indices[letter].push_back(i);
    }

    std::vector<int> M(text.size(), 0);


    // N-функия...............
    std::vector<std::string> reversed_pattern = pattern;
    reverseVector(reversed_pattern);
    std::vector<int> N = zFunction(reversed_pattern);
    reverseVector(N);
    //.........................

    // L-функция................
    std::vector<int> L(pattern_length, 0);
    for (int i = 0; i < pattern_length; i ++){
        if (N[i] != 0) {
            L[pattern_length - N[i]] = i + 1;
        }
    }
    //..........................
    // SP-функция................
    std::vector<int> Z = zFunction(pattern);
    std::vector<int> USP(pattern_length, 0);
    for (int i = pattern_length - 1; i >= 0; i--){
        int x = i + Z[i] - 1;
        USP[x] = Z[i];
    }
    //..........................

    int sphs_shift = 0;
    int l = 0;
    int text_length = text.size();
    int h = pattern_length - 1;
    int i = pattern_length - 1;
    int j = h;
    while (h < text_length){
        if (M[j] == 0 ){ 
            if (text[j] == pattern[i]){
                if (i == 0){
                    M[h] = pattern_length;
                    answer.push_back(j);

                    // int max_shift = 0;
                    // max_shift = pattern_length - USP[pattern_length - 1];

                    if (i + 1 == pattern_length){ // рассчитываем переменную для сдвига по спхс
                        sphs_shift = pattern_length; 
                    }
                    else if (L[i + 1] == 0){
                        sphs_shift = pattern_length;
                    }
                    else
                        sphs_shift = L[i + 1];
                    int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1});

                    h += max_shift;
                    j = h;
                    i = pattern_length - 1;
                }
                else{
                    i -= 1;
                    j -= 1;
                }
            }
            else{
                M[h] = h - j;
                
                if (i + 1 == pattern_length){ // рассчитываем переменную для сдвига по спхс
                    sphs_shift = pattern_length; 
                }
                else if (L[i + 1] == 0){ 
                    sphs_shift = pattern_length;
                }
                else
                    sphs_shift = L[i + 1];

                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1}); 
                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
        }
        else if (M[j] < N[i]){
            i -= M[j];
            j -= M[j];
        }
        else if (M[j] == N[i]){
            if (N[i] == i + 1){ 
                M[h] = h - j;
                answer.push_back(j - N[i] + 1);
                i -= N[i];
                // int max_shift = 0;
                // max_shift = pattern_length - USP[pattern_length - 1];

                if (i + 1 == pattern_length){ // рассчитываем переменную для сдвига по спхс
                    sphs_shift = pattern_length; 
                }
                else if (L[i + 1] == 0){
                    sphs_shift = pattern_length;
                }
                else
                    sphs_shift = L[i + 1];
                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1}); 

                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
            else{
                i -= M[j];
                j -= M[j];
            }
        }
        else if (M[j] > N[i]){
            if (N[i] == i + 1){ 
                M[h] = h - j;
                answer.push_back(j - N[i] + 1);
                i -= N[i];

                // int max_shift = 0;
                // max_shift = pattern_length - USP[pattern_length - 1];

                if (i + 1 == pattern_length){ // рассчитываем переменную для сдвига по спхс
                    sphs_shift = pattern_length; 
                }
                else if (L[i + 1] == 0){
                    sphs_shift = pattern_length;
                }
                else
                    sphs_shift = L[i + 1];
                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1});

                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
            else{
                M[h] = h - j;
                i -= N[i];

                if (i + 1 == pattern_length){ // рассчитываем переменную для сдвига по спхс
                    sphs_shift = pattern_length; 
                }
                else if (L[i + 1] == 0){ 
                    sphs_shift = pattern_length;
                }
                else
                    sphs_shift = L[i + 1];
                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1});

                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
        }
    }

    auto end_AG = std::chrono::system_clock::now();
    uint64_t apostolico_giancarlo_ts = std::chrono::duration_cast<duration_t>( end_AG - start_AG ).count();

    // for (int i = 0; i < answer.size(); i++){
    //     std::cout<<wordstring[answer[i]].first<<", "<<wordstring[answer[i]].second<<std::endl;
    // }
    

    std::chrono::time_point<std::chrono::system_clock> start_native = std::chrono::system_clock::now();

    std::vector<int> ander_native = naive_search(pattern, text);
    // for (int i = 0; i < ander_native.size(); i++){
    //     std::cout<<wordstring[ander_native[i]].first<<", "<<wordstring[ander_native[i]].second<<std::endl;
    // }

    auto end_native = std::chrono::system_clock::now();

    uint64_t native_ts = std::chrono::duration_cast<duration_t>( end_native - start_native ).count();
    std::cout << "Apostolico-Giancarlo algorithm time: " << apostolico_giancarlo_ts << DURATION_PREFIX << std::endl;
    std::cout << "native algorithm time: " << native_ts << DURATION_PREFIX << std::endl;


    return 0;
}

