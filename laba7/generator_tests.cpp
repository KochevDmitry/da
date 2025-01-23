#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

std::vector<std::vector<int>> generateMatrix(int n, int m) {
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = rand() % 2; // Генерируем случайно 0 или 1
        }
    }
    return matrix;
}

// Функция для записи теста в файл
void writeTestCaseToFile(const std::string& filename, int n, int m, const std::vector<std::vector<int>>& matrix) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << filename << " for writing!" << std::endl;
        return;
    }

    outFile << n << " " << m << std::endl;
    for (const auto& row : matrix) {
        for (int cell : row) {
            outFile << cell;
        }
        outFile << std::endl;
    }

    outFile.close();
    std::cout << "Test case written to " << filename << std::endl;
}

int main() {
    std::srand(std::time(0)); // Инициализация генератора случайных чисел

    int n = 500, m = 500;
    std::string filename = "test1.txt";


    // Генерация случайной матрицы
    std::vector<std::vector<int>> matrix = generateMatrix(n, m);

    // Запись сгенерированного теста в файл
    writeTestCaseToFile(filename, n, m, matrix);

    return 0;
}
