#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

// Генерация одного тестового случая
std::vector<int> generateTestCase(int numSegments, int minLength, int maxLength) {
    std::mt19937 rng(std::time(nullptr)); // Инициализация генератора случайных чисел
    std::uniform_int_distribution<int> dist(minLength, maxLength);

    std::vector<int> segments;
    for (int i = 0; i < numSegments; ++i) {
        segments.push_back(dist(rng));
    }
    return segments;
}

// Запись тестового случая в файл
void writeToFile(const std::vector<int>& testCase, const std::string& fileName) {
    std::ofstream output(fileName);
    if (!output.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }

    output << testCase.size() << '\n';
    for (auto segment : testCase) {
        output << segment << '\n';
    }

    output.close();
}

int main() {
    int N = 300; // Задайте нужное вам значение N
    int minLength = 1;
    int maxLength = 100000;

    std::vector<int> testCase = generateTestCase(N, minLength, maxLength);

    writeToFile(testCase, "test1.txt");

    std::cout << "Тестовые данные успешно сохранены в файле test1.txt." << std::endl;

    return 0;
}