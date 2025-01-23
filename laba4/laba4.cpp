#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <sstream>


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
// void reverseVector(std::vector<std::string>& v) {
    int n = v.size();
    for (int i = 0; i < n / 2; ++i) {
        std::swap(v[i], v[n - i - 1]);
    }
}

std::vector<int> zFunction(const std::vector<std::string>&  s){ //хорошо работает, но не уверен
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
        while(letter_indices[letter][i] < index && letter_indices[letter].size() > i){ // можно ускорить
            i += 1;
        }
        i -= 1;
        shift = index - letter_indices[letter][i];
    }
    return shift;
}

// int SPHS()

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
    // for(int i = 0; i < string_vector.size(); i++){
    //     std::cout<<string_vector[i]<<std::endl;
    // }

    std::vector<std::pair<int,int>> wordstring = processStrings(string_vector); 

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

    // for(int i = 0; i < text.size(); i++){
    //     std::cout<<text[i]<<" ";
    // }
    // std::cout<<std::endl;



    int pattern_length = pattern.size();
    // Создание словаря буква - вектор чисел
    // std::map<char, std::vector<int>> letter_indices;

    // Заполнение словаря
    for (int i = 0; i < pattern_length; ++i) {
        std::string letter = pattern[i];
        letter_indices[letter].push_back(i);
    }

    // for (const auto& pair : letter_indices) {
    //     std::string letter = pair.first;
    //     const std::vector<int>& indices = pair.second;
    //     std::cout << "Строка '" << letter << "' встречается в индексах: ";
    //     for (int index : indices) {
    //         std::cout << index << " ";
    //     }
    //     std::cout << std::endl;
    // }

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



    //алгоритм А-Д
    // std::cout << "Весь текст:" << std::endl << text << std::endl;
    // std::cout << "Паттерн: " << std::endl << pattern << std::endl;
    // std::vector<int> Z = zFunction(pattern);
    // std::cout << "Z-функция: ";
    // for (int i = 0; i < Z.size(); ++i) {
    //     std::cout << Z[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "N-функция: ";
    // for (int i = 0; i < N.size(); ++i) {
    //     std::cout << N[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "L-функция: ";
    // for (int i = 0; i < L.size(); ++i) {
    //     std::cout << L[i] << " ";
    // }
    // std::cout << std::endl;

    int sphs_shift = 0;
    int l = 0;
    int text_length = text.size();
    int h = pattern_length - 1;
    int i = pattern_length - 1;
    int j = h;
    while (h < text_length){
        // std::cout<<"h: "<<h<<"; i: "<<i<<"; j: "<<j<<"; l: "<<l<<std::endl;
        if (M[j] == 0 ){ //&& N[i] == 0
            // std::cout<<"case 1"<<std::endl;
            if (text[j] == pattern[i]){
                // std::cout<<"case 1 1"<<std::endl;
                if (i == 0){ // совпадение
                    // std::cout<<"case 1 1 1"<<std::endl;
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
                    int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1}); // сдвиг по БМ

                    h += max_shift;
                    j = h;
                    i = pattern_length - 1;
                }
                else{
                    // std::cout<<"case 1 1 2"<<std::endl;
                    i -= 1;
                    j -= 1;
                }
            }
            else{
                // std::cout<<"case 1 2"<<std::endl;
                M[h] = h - j;
                
                if (i + 1 == pattern_length){ // рассчитываем переменную для сдвига по спхс
                    sphs_shift = pattern_length; 
                }
                else if (L[i + 1] == 0){ 
                    sphs_shift = pattern_length;
                }
                else
                    sphs_shift = L[i + 1];

                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1}); // сдвиг по БМ
                // std::cout<<pattern_length - sphs_shift<<" "<<UPPS(text[j], i)<<std::endl;
                // std::cout<<"max_shift: "<<max_shift<<std::endl;

                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
        }
        else if (M[j] < N[i]){
            // std::cout<<"case 2"<<std::endl;
            i -= M[j];
            j -= M[j];
            // БМ или оставить так?
        }
        else if (M[j] == N[i]){
            // std::cout<<"case 3"<<std::endl;
            if (N[i] == i + 1){ // совпадение
                // std::cout<<"case 3 1"<<std::endl;
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
                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1}); // сдвиг по БМ

                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
            else{
                // std::cout<<"case 3 2"<<std::endl;
                i -= M[j];
                j -= M[j];
            }
        }
        else if (M[j] > N[i]){
            // std::cout<<"case 4"<<std::endl;
            if (N[i] == i + 1){ // совпадение
                // std::cout<<"case 4 1"<<std::endl;
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
                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1}); // сдвиг по БМ

                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
            else{
                // std::cout<<"case 4 2"<<std::endl;
                M[h] = h - j;

                //сдвиг после h-j+Ni
                i -= N[i];

                if (i + 1 == pattern_length){ // рассчитываем переменную для сдвига по спхс
                    sphs_shift = pattern_length; 
                }
                else if (L[i + 1] == 0){ 
                    sphs_shift = pattern_length;
                }
                else
                    sphs_shift = L[i + 1];
                int max_shift = std::max({UPPS(text[j], i), pattern_length - sphs_shift, 1}); // сдвиг по БМ
                // std::cout<<pattern_length - sphs_shift<<" "<<UPPS(text[j], i)<<std::endl;
                // std::cout<<"max_shift: "<<max_shift<<std::endl;

                h += max_shift;
                j = h;
                i = pattern_length - 1;
            }
        }

        // for (int s = 0; s < text.size(); s++)
        //     std::cout <<text[s]<<" ";
        // std::cout<<std::endl;
        // for (int s = 0; s < h + 1 - pattern_length; s++)
        //     std::cout <<"  ";   
        // for (int s = 0; s < pattern.size(); s++)
        //     std::cout <<pattern[s]<<" ";
        // std::cout<<std::endl;
        // for (int s = 0; s < M.size(); s++)
        //     std::cout <<M[s]<<" ";
        // std::cout<<std::endl;

    }
 
    // for (int i = 0; i < answer.size(); ++i) { // вывод всех вхождений, если текст в одной строке
    //     std::cout << answer[i] << " ";
    // }
    // std::cout<<std::endl;
    // if (!answer.empty()){ // неправильный вывод, почти работающий
    //     int q = 0, w = 0, e = 0;
    //     char last_letter;
    //     for (int i = 0; i < string_vector.size(); i++){
    //         last_letter = ' ';
    //         for (int j = 0; j < string_vector[i].length(); j++){
    //             // std::cout<<"q: "<<q<<"; i: "<<i<<"; j: "<<j<<"; w: "<<w<<std::endl;
    //             if (e == answer[q] && q < answer.size()){
    //                 std::cout<<i + 1<<", "<<w + 1<<std::endl;
    //                 q += 1;
    //             }
    //             if (string_vector[i][j] == ' ' && last_letter != ' '){
    //                 e += 1;
    //                 w += 1;
    //             }
    //             last_letter = string_vector[i][j];
    //         }
    //         e += 1;
    //         w = 0;
    //     }
    // }

    for (int i = 0; i < answer.size(); i++){
        std::cout<<wordstring[answer[i]].first<<", "<<wordstring[answer[i]].second<<std::endl;
    }

    return 0;
}

