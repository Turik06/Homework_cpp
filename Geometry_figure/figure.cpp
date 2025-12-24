#include "figure.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

Circle::Circle(Point c, double r) : center(c), radius(r) {}
double Circle::calc_area() const { return M_PI * radius * radius; }
double Circle::calc_perimetr() const { return 2 * M_PI * radius; }
void Circle::name() const { std::cout << "Фигура: Круг\n"; }

Ellipse::Ellipse(Point c, double a, double b) : center(c), a(a), b(b) {}
double Ellipse::calc_area() const { return M_PI * a * b; }
double Ellipse::calc_perimetr() const {
    return 2 * M_PI * std::sqrt((a * a + b * b) / 2);
}
void Ellipse::name() const { std::cout << "Фигура: Эллипс\n"; }

Triangle::Triangle(const std::vector<Point>& pts) : points(pts) {}
double Triangle::calc_area() const {
    double a = std::hypot(points[0].x - points[1].x, points[0].y - points[1].y);
    double b = std::hypot(points[1].x - points[2].x, points[1].y - points[2].y);
    double c = std::hypot(points[2].x - points[0].x, points[2].y - points[0].y);
    double p = (a + b + c) / 2;
    return std::sqrt(p * (p - a) * (p - b) * (p - c));
}
double Triangle::calc_perimetr() const {
    double sum = 0;
    for (int i = 0; i < 3; ++i)
        sum += std::hypot(points[i].x - points[(i + 1) % 3].x, points[i].y - points[(i + 1) % 3].y);
    return sum;
}
void Triangle::name() const { std::cout << "Фигура: Треугольник\n"; }

Rectangle::Rectangle(const std::vector<Point>& pts) : points(pts) {}
double Rectangle::calc_area() const {
    double a = std::hypot(points[0].x - points[1].x, points[0].y - points[1].y);
    double b = std::hypot(points[1].x - points[2].x, points[1].y - points[2].y);
    return a * b;
}
double Rectangle::calc_perimetr() const {
    double sum = 0;
    for (int i = 0; i < 4; ++i)
        sum += std::hypot(points[i].x - points[(i + 1) % 4].x, points[i].y - points[(i + 1) % 4].y);
    return sum;
}
void Rectangle::name() const { std::cout << "Фигура: Прямоугольник\n"; }

Polygon::Polygon(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка открытия файла.\n";
        return;
    }

    int n;
    file >> n;
    double x, y;
    for (int i = 0; i < n && file >> x >> y; ++i) {
        points.emplace_back(x, y);
    }
}

double Polygon::calc_area() const {
    double area = 0;
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        area += (points[i].x * points[(i + 1) % n].y) - (points[(i + 1) % n].x * points[i].y);
    }
    return std::fabs(area) / 2.0;
}

double Polygon::calc_perimetr() const {
    double peri = 0;
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        peri += std::hypot(points[i].x - points[(i + 1) % n].x, points[i].y - points[(i + 1) % n].y);
    }
    return peri;
}

void Polygon::name() const { std::cout << "Фигура: Многоугольник\n"; }

int Polygon::vertex_count() const {
    return points.size();
}
