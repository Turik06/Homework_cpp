#include "figure.h"
#include <iostream>
#include <vector>

int main() {
    setlocale(LC_ALL, "ru");

    int circle_count = 0, ellipse_count = 0, triangle_count = 0;
    int rectangle_count = 0, polygon_count = 0, scribble_count = 0;

    double circle_area_sum = 0, ellipse_area_sum = 0, triangle_area_sum = 0;
    double rectangle_area_sum = 0, polygon_area_sum = 0;

    double circle_perimeter_sum = 0, ellipse_perimeter_sum = 0, triangle_perimeter_sum = 0;
    double rectangle_perimeter_sum = 0, polygon_perimeter_sum = 0;

    std::vector<GeometricFigure*> figures;
 
    // Создаем несколько кругов
    figures.push_back(new Circle(Point(0, 0), 10));
    figures.push_back(new Circle(Point(5, 5), 7));
    figures.push_back(new Circle(Point(-3, 4), 12));

    // Создаем несколько эллипсов
    figures.push_back(new Ellipse(Point(1, 1), 15, 5));
    figures.push_back(new Ellipse(Point(2, -2), 8, 3));
    figures.push_back(new Ellipse(Point(0, 5), 10, 4));

    // Создаем несколько треугольников
    figures.push_back(new Triangle({ Point(0, 0), Point(0, 3), Point(4, 0) }));
    figures.push_back(new Triangle({ Point(1, 1), Point(4, 5), Point(7, 2) }));
    figures.push_back(new Triangle({ Point(-2, -2), Point(0, 3), Point(5, 0) }));

    // Создаем несколько прямоугольников
    figures.push_back(new Rectangle({ Point(0, 0), Point(0, 3), Point(5, 3), Point(5, 0) }));
    figures.push_back(new Rectangle({ Point(2, 2), Point(2, 7), Point(6, 7), Point(6, 2) }));
    figures.push_back(new Rectangle({ Point(-5, -5), Point(-5, 0), Point(0, 0), Point(0, -5) }));

    // Создаем многоугольник
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

        if (dynamic_cast<Circle*>(figures[i])) {
            circle_count++;
            circle_area_sum += area;
            circle_perimeter_sum += perimeter;
        }
        else if (dynamic_cast<Ellipse*>(figures[i])) {
            ellipse_count++;
            ellipse_area_sum += area;
            ellipse_perimeter_sum += perimeter;
        }
        else if (dynamic_cast<Triangle*>(figures[i])) {
            triangle_count++;
            triangle_area_sum += area;
            triangle_perimeter_sum += perimeter;
        }
        else if (dynamic_cast<Rectangle*>(figures[i])) {
            rectangle_count++;
            rectangle_area_sum += area;
            rectangle_perimeter_sum += perimeter;
        }
        else if (Polygon* p = dynamic_cast<Polygon*>(figures[i])) {
            polygon_count++;
            polygon_area_sum += area;
            polygon_perimeter_sum += perimeter;

            if (p->vertex_count() > 10)
                scribble_count++;
        }
    }

    std::cout << "\nИТОГИ:\n";
    std::cout << "Кругов: " << circle_count
        << ", Суммарная площадь: " << circle_area_sum
        << ", Суммарный периметр: " << circle_perimeter_sum << "\n";

    std::cout << "Эллипсов: " << ellipse_count
        << ", Суммарная площадь: " << ellipse_area_sum
        << ", Суммарный периметр: " << ellipse_perimeter_sum << "\n";

    std::cout << "Треугольников: " << triangle_count
        << ", Суммарная площадь: " << triangle_area_sum
        << ", Суммарный периметр: " << triangle_perimeter_sum << "\n";

    std::cout << "Прямоугольников: " << rectangle_count
        << ", Суммарная площадь: " << rectangle_area_sum
        << ", Суммарный периметр: " << rectangle_perimeter_sum << "\n";

    std::cout << "Многоугольников: " << polygon_count
        << ", Суммарная площадь: " << polygon_area_sum
        << ", Суммарный периметр: " << polygon_perimeter_sum << "\n";

    std::cout << "Многоугольников-каракули (больше 10 вершин): " << scribble_count << "\n";

    // Освобождение памяти
    for (size_t i = 0; i < figures.size(); ++i) {
        delete figures[i];
    }

    return 0;
}
