#ifndef _RATIONAL_H
#define _RATIONAL_H

#include <iostream>
#include <cmath>

class Rational {
private:
    int numer;
    int denom;
    void simplify(); // Упрощение дроби

public:
    Rational();
    Rational(int n);
    Rational(int n, int d);

    // Арифметические операторы
    Rational& operator +=(const Rational& r);
    Rational operator +(const Rational& r) const;
    Rational operator -() const;
    Rational& operator -=(const Rational& r);
    Rational operator -(const Rational& r) const;
    Rational& operator *=(const Rational& r);
    Rational operator *(const Rational& r) const;
    Rational& operator *=(int num);
    friend Rational operator*(const Rational& r, int num);
    friend Rational operator*(int num, const Rational& r);
    Rational& operator /=(const Rational& r);
    Rational& operator /=(int num); // Деление на целое число
    Rational operator /(const Rational& r) const;
    friend Rational operator/(const Rational& r, int num);
    friend Rational operator/(int num, const Rational& r);

    // Операторы инкремента
    Rational& operator++();    // Префиксный инкремент
    Rational operator++(int);  // Постфиксный инкремент

    // Операторы сравнения
    bool operator ==(const Rational& r) const;
    bool operator !=(const Rational& r) const;
    bool operator < (const Rational& r) const;
    bool operator <=(const Rational& r) const;
    bool operator > (const Rational& r) const;
    bool operator >=(const Rational& r) const;

    // Преобразования типов
    operator int() const;
    operator double() const;

    // Операторы ввода/вывода
    friend std::istream& operator >>(std::istream& in, Rational& r);
    friend std::ostream& operator <<(std::ostream& out, const Rational& r);
};

#endif // _RATIONAL_H
