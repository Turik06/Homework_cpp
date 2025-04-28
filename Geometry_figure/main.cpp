#include "figure.h"
#include <iostream>
#include <vector>
#include <map>

int main() {
    setlocale(LC_ALL, "ru");

    std::map<std::string, int> figure_count;
    std::map<std::string, double> area_sum;
    std::map<std::string, double> perimeter_sum;

    int scribble_count = 0;

    std::vector<GeometricFigure*> figures;

    // Создаем несколько фигур
    figures.push_back(new Circle(Point(0, 0), 10));
    figures.push_back(new Circle(Point(5, 5), 7));
    figures.push_back(new Circle(Point(-3, 4), 12));

    figures.push_back(new Ellipse(Point(1, 1), 15, 5));
    figures.push_back(new Ellipse(Point(2, -2), 8, 3));
    figures.push_back(new Ellipse(Point(0, 5), 10, 4));

    figures.push_back(new Triangle({ Point(0, 0), Point(0, 3), Point(4, 0) }));
    figures.push_back(new Triangle({ Point(1, 1), Point(4, 5), Point(7, 2) }));
    figures.push_back(new Triangle({ Point(-2, -2), Point(0, 3), Point(5, 0) }));

    figures.push_back(new Rectangle({ Point(0, 0), Point(0, 3), Point(5, 3), Point(5, 0) }));
    figures.push_back(new Rectangle({ Point(2, 2), Point(2, 7), Point(6, 7), Point(6, 2) }));
    figures.push_back(new Rectangle({ Point(-5, -5), Point(-5, 0), Point(0, 0), Point(0, -5) }));

    Polygon* poly1 = new Polygon("granitsy-uchastka2.txt");
    if (poly1->vertex_count() >= 3) figures.push_back(poly1);
    else delete poly1;

    // Обработка всех фигур
    for (size_t i = 0; i < figures.size(); ++i) {
        figures[i]->name();
        double area = figures[i]->calc_area();
        double perimeter = figures[i]->calc_perimetr();

        std::cout << "Площадь: " << area
            << ", Периметр: " << perimeter << "\n";

        std::string type_name;

        if (dynamic_cast<Circle*>(figures[i])) type_name = "Круг";
        else if (dynamic_cast<Ellipse*>(figures[i])) type_name = "Эллипс";
        else if (dynamic_cast<Triangle*>(figures[i])) type_name = "Треугольник";
        else if (dynamic_cast<Rectangle*>(figures[i])) type_name = "Прямоугольник";
        else if (Polygon* p = dynamic_cast<Polygon*>(figures[i])) {
            type_name = "Многоугольник";
            if (p->vertex_count() > 10) scribble_count++;
        }

        figure_count[type_name]++;
        area_sum[type_name] += area;
        perimeter_sum[type_name] += perimeter;
    }

    // Вывод итогов
    std::cout << "\nИТОГИ:\n";
    for (const auto& pair : figure_count) {
        const std::string& name = pair.first;
        std::cout << name << "ов: " << pair.second
            << ", Суммарная площадь: " << area_sum[name]
            << ", Суммарный периметр: " << perimeter_sum[name] << "\n";
    }

    std::cout << "Многоугольников-каракули (больше 10 вершин): " << scribble_count << "\n";

    // Освобождение памяти
    for (size_t i = 0; i < figures.size(); ++i) {
        delete figures[i];
    }

    return 0;
}
