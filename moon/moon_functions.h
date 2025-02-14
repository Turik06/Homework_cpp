#ifndef MOON_FUNCTIONS_H
#define MOON_FUNCTIONS_H

#include <cmath> 

// Объявление структуры MoonData
struct MoonData {
    char date[9];
    char time[7];
    double t_hours;
    double elevation;
};

// Прототипы функций (объявления)
int my_strlen(const char* s);
bool stringsEqual(const char* a, const char* b);
void my_strcpy(char* dest, const char* src);
void my_strcat(char* dest, const char* src);

void convertDateFormat(const char* ddmm, char* target);
double timeToHours(const char* hms);
void hoursToHMS(double hours, char* hms);

bool invertMatrix(const double m[3][3], double inv[3][3]);
void multiplyMatrixVector(const double m[3][3], const double v[3], double result[3]);

#endif // MOON_FUNCTIONS_H#pragma once
