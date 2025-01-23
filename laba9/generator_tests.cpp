#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;

// Функция для генерации случайного графа
void generateGraph(int n, int m, const string& filename) {
    if (m > n * (n - 1) / 2) {
        cout << "Слишком много ребер для заданного количества вершин." << endl;
        return;
    }

    ofstream outFile(filename); // Открываем файл для записи
    if (!outFile.is_open()) {
        cout << "Ошибка при открытии файла!" << endl;
        return;
    }

    outFile << n << " " << m << endl; // Сохраняем количество вершин и ребер

    set<pair<int, int>> edges; // Множество для хранения уникальных ребер

    srand(time(0)); // Устанавливаем случайное зерно для генерации

    // Генерация ребер
    while (edges.size() < m) {
        int u = rand() % n + 1; // Случайная вершина от 1 до n
        int v = rand() % n + 1;

        if (u != v) {
            // Упорядочиваем пару (меньший номер вершины первым)
            if (u > v) swap(u, v);

            // Добавляем уникальное ребро
            if (edges.find({u, v}) == edges.end()) {
                edges.insert({u, v});
                outFile << u << " " << v << endl;
            }
        }
    }

    outFile.close(); // Закрываем файл
    cout << "Тест сгенерирован и сохранен в " << filename << endl;
}

int main() {
    int n = 100000, m = 100000;

    string filename = "test1.txt";
    generateGraph(n, m, filename);

    return 0;
}
