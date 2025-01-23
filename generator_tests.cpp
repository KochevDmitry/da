#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <set>
#include <algorithm>

int sum_point = 0;
int sum_another_point = 0;

struct Point {
    int x, y;

    // Оператор < для сравнения точек
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

// Генерация случайного числа в заданном диапазоне
int random_int(int low, int high) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(low, high);
    return dist(rng);
}

// Генерация случайного выпуклого многоугольника
std::vector<Point> generate_convex_polygon(int num_points, int min_x, int max_x, int min_y, int max_y) {
    std::set<Point> points_set;

    // Создание множества случайных уникальных точек
    while (points_set.size() < static_cast<size_t>(num_points)) {
        int x = random_int(min_x, max_x);
        int y = random_int(min_y, max_y);
        points_set.insert({x, y});
    }

    // std::cout<<"points_set: "<<points_set.size()<<std::endl;
    sum_another_point += points_set.size();


    std::vector<Point> points(points_set.begin(), points_set.end());

    // Сортируем точки для создания выпуклой оболочки
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    // std::cout<<"points_vector: "<<points.size()<<std::endl;

    // Используем алгоритм Грэхема для построения выпуклой оболочки
    std::vector<Point> hull;

    // Нижняя часть оболочки
    for (const auto& p : points) {
        while (hull.size() >= 2 && 
               (hull[hull.size() - 1].x - hull[hull.size() - 2].x) * (p.y - hull[hull.size() - 2].y) -
               (hull[hull.size() - 1].y - hull[hull.size() - 2].y) * (p.x - hull[hull.size() - 2].x) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    // Верхняя часть оболочки
    size_t t = hull.size() + 1;
    for (auto it = points.rbegin(); it != points.rend(); ++it) {
        const auto& p = *it;
        while (hull.size() >= t && 
               (hull[hull.size() - 1].x - hull[hull.size() - 2].x) * (p.y - hull[hull.size() - 2].y) -
               (hull[hull.size() - 1].y - hull[hull.size() - 2].y) * (p.x - hull[hull.size() - 2].x) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    hull.pop_back(); // Удаляем дублирующую точку
    // std::cout<<"hull size: "<< hull.size()<<std::endl;
    return hull;
}

// Основная функция генерации тестов
void generate_test(int total_points, int num_check_points, int min_coord, int max_coord, const std::string& filename) {
    int remaining_points = total_points;
    std::vector<std::vector<Point>> polygons;
    std::vector<Point> check_points;

    // Координаты для размещения многоугольников без пересечений
    int offset_x = min_coord, offset_y = min_coord;
    int step = (max_coord - min_coord) / 100;  // Сделаем шаг большим, чтобы многоугольники не перекрывались.

    // Генерация случайного распределения вершин среди многоугольников
    std::vector<int> polygon_sizes;
    int num_polygons = 0;

    // Генерация многоугольников с минимальным количеством точек (например, 3) и случайным количеством для каждого
    while (remaining_points >= 3) { // Мы уверены, что остаётся хотя бы 3 точки для одного многоугольника
        int min_points = 3;
        int max_points = std::min(remaining_points, 25); // Максимум 25 точек в одном многоугольнике
        if (max_points < min_points) break;  // Если оставшихся точек недостаточно для минимального многоугольника, выходим

        int num_points = random_int(min_points, max_points); // Генерируем количество точек для текущего многоугольника
        polygon_sizes.push_back(num_points);
        remaining_points -= num_points;
        num_polygons++;
    }
    // for (int i = 0; i < polygon_sizes.size(); i++){
    //     std::cout<<polygon_sizes[i]<<" ";
    // }
    std::cout<<std::accumulate(polygon_sizes.begin(), polygon_sizes.end(), 0)<<std::endl;


    // Если оставшиеся точки меньше 3, их не распределяем
    // if (remaining_points > 0) {
    //     std::cerr << "Warning: Not all points have been distributed among polygons (remaining points: " << remaining_points << ")\n";
    // }

    // Генерация многоугольников
    for (int i = 0; i < num_polygons; ++i) {
        auto polygon = generate_convex_polygon(polygon_sizes[i], offset_x + i * step, offset_x + (i + 1) * step, offset_y + i * step, offset_y + (i + 1) * step);
        polygons.push_back(polygon);
    }
    // std::cout<<sum_another_point<<std::endl;

    // Генерация точек на проверку
    for (int i = 0; i < num_check_points; ++i) {
        int x = random_int(min_coord, max_coord);
        int y = random_int(min_coord, max_coord);
        check_points.push_back({x, y});
    }

    // Открытие файла для записи
    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        // std::cerr << "Error: Could not open file for writing: " << filename << "\n";
        return;
    }

    // Запись тестов в файл
    out_file << num_polygons << " " << num_check_points << "\n";
    for (const auto& polygon : polygons) {
        out_file << polygon.size();
        sum_point += polygon.size();
        for (const auto& point : polygon) {
            out_file << " " << point.x << " " << point.y;
        }
        out_file << "\n";
    }
    std::cout<<"all_size: "<<sum_point<<std::endl;
    for (const auto& point : check_points) {
        out_file << point.x << " " << point.y << "\n";
    }

    // Закрытие файла
    out_file.close();
}

int main() {
    int total_points = 220000; // Общее число точек во всех многоугольниках, подбираем так, чтобы all_size был равен желаемому количеству точек
    int num_check_points = 100000; // Количество точек на проверку
    int min_coord = 0, max_coord = 100000; // Диапазон координат
    std::string filename = "test.txt";  // Имя выходного файла

    generate_test(total_points, num_check_points, min_coord, max_coord, filename);

    std::cout << "Test data has been written to " << filename << "\n";
    return 0;
}
