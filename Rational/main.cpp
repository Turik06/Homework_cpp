#include <iostream>
#include <cmath>
#include "rational.h"

using namespace std;

void inputNonZeroRational(Rational& a) {
    do {
        cout << "Введите коэффициент a (a != 0): ";
        cin >> a;
    } while (a == Rational(0));
}

int main() {
    setlocale(LC_ALL, "ru");

    Rational a, b, c, D_rational;
    int choice;

    cout << "Выберите тип уравнения (линейное - 0, квадратное - 1): ";
    cin >> choice;

    if (choice == 0) {
        cout << "Решение линейного уравнения вида: a*x + b = 0" << endl;
        inputNonZeroRational(a);
        cout << "Введите коэффициент b: ";
        cin >> b;
        Rational x = -b / a;
        cout << "x = " << x << endl;
    }
    else if (choice == 1) {
        cout << "Решение квадратного уравнения вида: a*x^2 + b*x + c = 0" << endl;
        inputNonZeroRational(a);
        cout << "Введите коэффициент b: ";
        cin >> b;
        cout << "Введите коэффициент c: ";
        cin >> c;

        D_rational = b * b - 4 * a * c;
        double D = static_cast<double>(D_rational); // Преобразование в double для проверки

        if (D < 0) {
            cout << "Нет вещественных корней" << endl;
        }
        else if (D == 0) {
            Rational x = -b / (Rational(2) * a);
            cout << "Единственный корень (кратный): " << x << endl;
        }
        else {
            double sqrt_D = sqrt(D); // Вычисление корня из дискриминанта
            Rational x1 = (-b + Rational(sqrt_D)) / (Rational(2) * a);
            Rational x2 = (-b - Rational(sqrt_D)) / (Rational(2) * a);
            cout << "Два вещественных корня:" << endl;
            cout << "x1 = " << x1 << endl;
            cout << "x2 = " << x2 << endl;
        }
    }
    return 0;
}
