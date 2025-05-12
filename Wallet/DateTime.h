#ifndef DATETIME_H
#define DATETIME_H

#include <iostream>
using namespace std;

class DateException {};

class DateTime {
private:
    int year, month, day, hour, minute, second;
    bool isLeapYear(int y);
    bool isValidDate(int d, int m, int y);
    double toJulianDay() const;
    void fromJulianDay(double jd);

public:
    DateTime(int y = 2000, int m = 1, int d = 1, int h = 0, int min = 0, int s = 0);

    ~DateTime();
 

    void print() const;
    void printISO() const;
    int getDayOfWeek() const;
    static DateTime getEasterDate(int year);

    DateTime operator+(int days) const;
    DateTime& operator+=(int days);
    DateTime operator-(int days) const;
    DateTime& operator-=(int days);
    int differenceInDays(const DateTime& other) const;

    friend ostream& operator<<(ostream& os, const DateTime& dt);
    friend istream& operator>>(istream& is, DateTime& dt);
};

#endif