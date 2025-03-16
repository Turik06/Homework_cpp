#include "Datetime.h"
#include <iostream>
using namespace std;


const char* months[] = { "января", "февраля", "марта", "апреля", "мая", "июня",
                        "июля", "августа", "сентября", "октября", "ноября", "декабря" };
const char* daysOfWeek[] = { "воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота" };

bool DateTime::isLeapYear(int y) {
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

bool DateTime::isValidDate(int d, int m, int y) {
    if (m < 1 || m > 12 || d < 1)
        return false;
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (isLeapYear(y)) daysInMonth[1] = 29;
    return d <= daysInMonth[m - 1];
}

// Поддержка отрицательных лет до н.э.
double DateTime::toJulianDay() const {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    if (year < 0) y += 1;
    int m = month + 12 * a - 3;
    int jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    double timeFraction = (hour / 24.0) + (minute / 1440.0) + (second / 86400.0);
    return jdn + timeFraction;
}

void DateTime::fromJulianDay(double jd) {
    int jdn = static_cast<int>(jd);
    double timeFraction = jd - jdn;

    int l = jdn + 68569;
    int n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    int i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    int j = (80 * l) / 2447;
    day = l - (2447 * j) / 80;
    l = j / 11;
    month = j + 2 - 12 * l;
    year = 100 * (n - 49) + i + l;
    if (year <= 0) year -= 1;

    hour = static_cast<int>(timeFraction * 24);
    minute = static_cast<int>((timeFraction * 1440)) % 60;
    second = static_cast<int>((timeFraction * 86400)) % 60;
}

DateTime::DateTime(int y, int m, int d, int h, int min, int s) {
    if (!isValidDate(d, m, y) || h < 0 || h > 23 || min < 0 || min > 59 || s < 0 || s > 59)
        throw DateException();
    year = y; month = m; day = d;
    hour = h; minute = min; second = s;
}

void DateTime::print() const {
    cout << day << " " << months[month - 1] << " " << (year < 0 ? -year : year) << (year < 0 ? " до н.э." : "")
        << ", " << daysOfWeek[getDayOfWeek()] << " "
        << hour << ":" << minute << ":" << second << endl;
}

void DateTime::printISO() const {
    cout << year << "-" << (month < 10 ? "0" : "") << month << "-"
        << (day < 10 ? "0" : "") << day << "T"
        << (hour < 10 ? "0" : "") << hour << ":"
        << (minute < 10 ? "0" : "") << minute << ":"
        << (second < 10 ? "0" : "") << second << endl;
}

int DateTime::getDayOfWeek() const {
    return (static_cast<int>(toJulianDay() + 1)) % 7;
}

DateTime DateTime::operator+(int days) const {
    DateTime result = *this;
    result.fromJulianDay(this->toJulianDay() + days);
    return result;
}

DateTime& DateTime::operator+=(int days) {
    fromJulianDay(toJulianDay() + days);
    return *this;
}

DateTime DateTime::operator-(int days) const {
    DateTime result = *this;
    result.fromJulianDay(this->toJulianDay() - days);
    return result;
}

DateTime& DateTime::operator-=(int days) {
    fromJulianDay(toJulianDay() - days);
    return *this;
}

int DateTime::differenceInDays(const DateTime& other) const {
    return static_cast<int>(toJulianDay() - other.toJulianDay());
}

DateTime DateTime::getEasterDate(int year) {
    int a = year % 19;
    int b = year / 100;
    int c = year % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;
    int month = (h + l - 7 * m + 114) / 31;
    int day = ((h + l - 7 * m + 114) % 31) + 1;

    return DateTime(year, month, day, 0, 0, 0);
}

istream& operator>>(istream& is, DateTime& dt) {
    int y, m, d, h, min, s;
    is >> y >> m >> d >> h >> min >> s;
    dt = DateTime(y, m, d, h, min, s);
    return is;
}

ostream& operator<<(ostream& os, const DateTime& dt) {
    dt.print();
    return os;
}
