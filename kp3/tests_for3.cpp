#include <iostream>
#include <fstream>
#include <random>

// Генератор случайных чисел с заданным диапазоном
int random_int(int min_val, int max_val, std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(min_val, max_val);
    return dist(gen);
}

void generate_test(int n, int m, int x_min, int x_max, int y_min, int y_max, std::ostream& output) {
    std::mt19937 gen(std::random_device{}());

    // Записываем n и m
    output << n << " " << m << "\n";

    // Генерация отрезков
    for (int i = 0; i < n; ++i) {
        int l = random_int(x_min, x_max - 1, gen); // Левая граница отрезка
        int r = random_int(l + 1, x_max, gen);     // Правая граница отрезка
        int h = random_int(y_min, y_max, gen);     // Высота отрезка

        output << l << " " << r << " " << h << "\n";
    }

    // Генерация точек
    for (int i = 0; i < m; ++i) {
        int x = random_int(x_min, x_max, gen); // Координата x точки
        int y = random_int(y_min, y_max, gen); // Координата y точки

        output << x << " " << y << "\n";
    }
}

int main() {
    int n = 100000;  
    int m = 100000;   
    int x_min = -1000000000; 
    int x_max = 1000000000; 
    int y_min = -1000000000; 
    int y_max = 1000000000;  

    // Генерация теста и вывод его в файл test_input.txt
    std::ofstream output("test1_for3.txt");
    generate_test(n, m, x_min, x_max, y_min, y_max, output);
    output.close();

    std::cout << "Тест сгенерирован и записан в файл test1_for3.txt\n";
    return 0;
}
