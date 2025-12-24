#include "moon_functions.h" 


// Вычисление длины строки (аналог strlen)
int my_strlen(const char* s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Сравнение двух строк (возвращает true, если строки идентичны)
bool stringsEqual(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i])
            return false;
        i++;
    }
    return (a[i] == b[i]);
}

// Копирование строки (аналог strcpy)
void my_strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Конкатенация строк (аналог strcat). Предполагается, что dest имеет достаточный размер.
void my_strcat(char* dest, const char* src) {
    int len = my_strlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[len + i] = src[i];
        i++;
    }
    dest[len + i] = '\0';
}

// --- Функции для преобразования даты и времени --- //

// Преобразование даты из формата "ДД.ММ.ГГГГ" в "ГГГГММДД"
void convertDateFormat(const char* ddmm, char* target) {
    // Копируем год (позиции 6-9)
    target[0] = ddmm[6];
    target[1] = ddmm[7];
    target[2] = ddmm[8];
    target[3] = ddmm[9];
    // Месяц (позиции 3-4)
    target[4] = ddmm[3];
    target[5] = ddmm[4];
    // День (позиции 0-1)
    target[6] = ddmm[0];
    target[7] = ddmm[1];
    target[8] = '\0';
}

// Преобразование времени из формата "ЧЧММСС" в число часов (double)
double timeToHours(const char* hms) {
    int hours = (hms[0] - '0') * 10 + (hms[1] - '0');
    int minutes = (hms[2] - '0') * 10 + (hms[3] - '0');
    int seconds = (hms[4] - '0') * 10 + (hms[5] - '0');
    return hours + minutes / 60.0 + seconds / 3600.0;
}

// Преобразование часов (double) в строку формата "ЧЧММСС"
// Результат записывается в массив hms (размер не менее 7 символов)
void hoursToHMS(double hours, char* hms) {
    int h = (int)hours;
    double remainder = hours - h;
    double minutesTotal = remainder * 60.0;
    int m = (int)minutesTotal;
    double remainderSeconds = minutesTotal - m;
    int s = (int)(remainderSeconds * 60.0 + 0.5); // округление

    if (s >= 60) { s -= 60; m++; }
    if (m >= 60) { m -= 60; h++; }

    hms[0] = '0' + (h / 10);
    hms[1] = '0' + (h % 10);
    hms[2] = '0' + (m / 10);
    hms[3] = '0' + (m % 10);
    hms[4] = '0' + (s / 10);
    hms[5] = '0' + (s % 10);
    hms[6] = '\0';
}

// --- Математические функции --- //

// Инверсия матрицы 3x3. Если матрица имеет определитель, равный нулю, возвращает false.
bool invertMatrix(const double m[3][3], double inv[3][3]) {
    double det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
        - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
        + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    if (fabs(det) < 1e-9)
        return false;
    inv[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / det;
    inv[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) / det;
    inv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det;
    inv[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) / det;
    inv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / det;
    inv[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) / det;
    inv[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / det;
    inv[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) / det;
    inv[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / det;
    return true;
}

// Умножение матрицы 3x3 на вектор из 3 элементов.
void multiplyMatrixVector(const double m[3][3], const double v[3], double result[3]) {
    for (int i = 0; i < 3; i++) {
        result[i] = m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2];
    }
}