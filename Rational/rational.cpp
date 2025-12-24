#include "rational.h"
#include <iostream>

// Функция для вычисления НОД (наибольший общий делитель)
int gcd(int a, int b) {
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void Rational::simplify() {
    if (denom == 0) {
        numer = 0;
        denom = 1;
        return;
    }
    if (denom < 0) {
        numer = -numer;
        denom = -denom;
    }
    int commonDivisor = gcd(numer, denom);
    if (commonDivisor != 0) {
        numer /= commonDivisor;
        denom /= commonDivisor;
    }
}

Rational::Rational() : numer(0), denom(1) {}

Rational::Rational(int number) : numer(number), denom(1) {}

Rational::Rational(int n, int d) : numer(n), denom(d) {
    simplify();
}

Rational& Rational::operator +=(const Rational& r) {
    numer = numer * r.denom + denom * r.numer;
    denom *= r.denom;
    simplify();
    return *this;
}

Rational Rational::operator +(const Rational& r) const {
    Rational res(*this);
    return res += r;
}

Rational Rational::operator -() const {
    return { -numer, denom };
}

Rational& Rational::operator -=(const Rational& r) {
    return (*this += (-r));
}

Rational Rational::operator -(const Rational& r) const {
    Rational res(*this);
    return res -= r;
}

Rational& Rational::operator *=(const Rational& r) {
    numer *= r.numer;
    denom *= r.denom;
    simplify();
    return *this;
}

Rational Rational::operator *(const Rational& r) const {
    Rational res(*this);
    return res *= r;
}

Rational& Rational::operator *=(int num) {
    numer *= num;
    simplify();
    return *this;
}

Rational operator*(const Rational& r, int num) {
    Rational res(r);
    return res *= num;
}

Rational operator*(int num, const Rational& r) {
    return r * num;
}

Rational& Rational::operator /=(const Rational& r) {
    if (r.numer == 0) {
        denom = 0; // Деление на 0
        return *this;
    }
    numer *= r.denom;
    denom *= r.numer;
    simplify();
    return *this;
}

Rational& Rational::operator /=(int num) {
    if (num == 0) {
        denom = 0; // Деление на 0
        return *this;
    }
    denom *= num;
    simplify();
    return *this;
}

Rational Rational::operator /(const Rational& r) const {
    Rational res(*this);
    return res /= r;
}

Rational operator/(const Rational& r, int num) {
    Rational res(r);
    return res /= num;
}

Rational operator/(int num, const Rational& r) {
    return Rational(num) / r;
}

Rational& Rational::operator++() {
    numer += denom;
    return *this;
}

Rational Rational::operator++(int) {
    Rational temp(*this);
    numer += denom;
    return temp;
}

bool Rational::operator ==(const Rational& r) const {
    return (numer == r.numer && denom == r.denom);
}

bool Rational::operator !=(const Rational& r) const {
    return !(*this == r);
}

bool Rational::operator <(const Rational& r) const {
    return numer * r.denom < r.numer * denom;
}

bool Rational::operator <=(const Rational& r) const {
    return numer * r.denom <= r.numer * denom;
}

bool Rational::operator >(const Rational& r) const {
    return numer * r.denom > r.numer * denom;
}

bool Rational::operator >=(const Rational& r) const {
    return numer * r.denom >= r.numer * denom;
}

Rational::operator int() const {
    return numer / denom;
}

Rational::operator double() const {
    return static_cast<double>(numer) / denom;
}

std::istream& operator >>(std::istream& in, Rational& r) {
    int n, d = 1;
    char slash;

    in >> n;
    if (in.peek() == '/') {
        in.get();
        in >> d;
    }

    if (d == 0) {
        in.setstate(std::ios::failbit);
        return in;
    }

    r = Rational(n, d);
    return in;
}

std::ostream& operator <<(std::ostream& out, const Rational& r) {
    out << r.numer << "/" << r.denom;
    return out;
}
