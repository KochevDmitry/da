#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

// Функция для генерации случайного слова
std::string generate_random_word(int max_length) {
    int length = rand() % max_length + 1;
    std::string word;
    for (int i = 0; i < length; ++i) {
        char letter = 'a' + rand() % 26;
        word += letter;
    }
    return word;
}

// Функция для генерации вектора случайных слов
std::vector<std::string> generate_random_words(int num_words, int max_length) {
    std::vector<std::string> words;
    for (int i = 0; i < num_words; ++i) {
        words.push_back(generate_random_word(max_length));
    }
    return words;
}

// Функция для записи теста в файл
void write_test_to_file(const std::string& filename, const std::vector<std::string>& pattern, const std::vector<std::string>& text) {
    std::ofstream outfile(filename);

    // Запись паттерна
    for (const std::string& word : pattern) {
        outfile << word << " ";
    }
    outfile << "\n";

    // Запись текста
    int line_length = 0;
    for (const std::string& word : text) {
        if (line_length + word.length() > 80) {
            outfile << "\n";
            line_length = 0;
        }
        outfile << word << " ";
        line_length += word.length() + 1;
    }
    outfile << "\n";

    outfile.close();
}

int main() {
    srand(time(0));

    // Настройки генерации теста
    int pattern_size = 1000; // Количество слов в паттерне
    int text_size = 100000;   // Количество слов в тексте
    int max_word_length = 16; // Максимальная длина слова
    double inclusion_rate = 0.3; // Процент вхождений паттерна в текст

    // Генерация паттерна
    std::vector<std::string> pattern = generate_random_words(pattern_size, max_word_length);

    // Генерация текста с учетом включения паттерна
    std::vector<std::string> text;
    for (int i = 0; i < text_size; ++i) {
        // Случайный выбор между паттерном и случайным словом
        if ((double)rand() / RAND_MAX < inclusion_rate) {
            i += pattern_size;
            text.insert(text.end(), pattern.begin(), pattern.end());
        } else {
            text.push_back(generate_random_word(max_word_length));
        }
    }

    // Запись теста в файл
    std::string filename = "test.txt";
    write_test_to_file(filename, pattern, text);

    std::cout << "Тест сохранен в файл " << filename << std::endl;

    return 0;
}
