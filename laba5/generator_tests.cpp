#include <iostream>
#include <vector>
#include <cstdlib>  // Для rand() и srand()
#include <ctime>    // Для time() (чтобы использовать текущее время как seed)
#include <string>
#include <algorithm> // Для std::shuffle
#include <random>    // Для std::random_device
#include <fstream>   // Для работы с файлами

// Функция для генерации случайного символа
char getRandomChar() {
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    return alphabet[rand() % alphabet.size()];
}

// Функция для генерации случайного текста длиной len
std::string generateRandomText(int len) {
    std::string text;
    for (int i = 0; i < len; ++i) {
        text += getRandomChar();
    }
    return text;
}

// Функция для генерации случайного паттерна (поисковой строки)
std::string generateRandomPattern(const std::string& text, int maxLength) {
    int len = rand() % maxLength + 1; // Длина паттерна от 1 до maxLength
    int start = rand() % (text.size() - len + 1); // Стартовая позиция для паттерна
    return text.substr(start, len);
}

// Основная функция для генерации тестов и записи их в файл
void generateTest(int textLength, int numPatterns, const std::string& filename) {
    // Открытие файла для записи
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для записи." << std::endl;
        return;
    }

    // Генерация случайного текста
    std::string text = generateRandomText(textLength);
    outputFile <<text << "\n";

    // Генерация паттернов
    std::vector<std::string> patterns;

    // Генерируем много коротких паттернов
    for (int i = 0; i < numPatterns / 2; ++i) {
        patterns.push_back(generateRandomPattern(text, 3)); // Короткие строки длиной до 3 символов
    }

    // Генерируем несколько длинных паттернов
    for (int i = 0; i < numPatterns / 2; ++i) {
        patterns.push_back(generateRandomPattern(text, textLength / 2)); // Длинные строки длиной до половины текста
    }

    // Перемешиваем паттерны для разнообразия
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(patterns.begin(), patterns.end(), g);

    // Записываем паттерны в файл
    for (const auto& pattern : patterns) {
        outputFile << pattern << "\n";
    }

    // Закрытие файла
    outputFile.close();
    std::cout << "Тесты успешно записаны в файл: " << filename << std::endl;
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел

    int textLength = 10000, numPatterns = 1000;
    std::string filename = "test1.txt";
    

    // Генерация тестов и запись в файл
    generateTest(textLength, numPatterns, filename);

    return 0;
}
