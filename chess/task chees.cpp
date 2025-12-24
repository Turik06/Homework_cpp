#include <iostream>
#include <fstream>
#include <locale.h>
#include <cmath>

using namespace std;

bool readQueens(const char* fileName, int* qx, int* qy);
bool isThreaten(int qx1, int qy1, int qx2, int qy2, const int* qx, const int* qy, int n);
void writePair(ofstream& out, int qx1, int qy1, int qx2, int qy2);
int readVert(ifstream& in);
int readHoris(ifstream& in);

int main()
{
    setlocale(LC_ALL, "Russian");

    int qx[8], qy[8];
    if (!readQueens("step.txt", qx, qy))
    {
        cout << "Не удалось прочитать координаты" << endl;
        return -1;
    }

    ofstream out("out.txt");
    if (!out.is_open())
    {
        cout << "Не удалось создать выходной файл" << endl;
        return -2;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = i + 1; j < 8; j++)
        {
            if (isThreaten(qx[i], qy[i], qx[j], qy[j], qx, qy, 8))
                writePair(out, qx[i], qy[i], qx[j], qy[j]);
        }
    }

    return 0;
}

int readVert(ifstream& in)
{
    char ch;
    in >> ch;
    if (ch < 'a' || ch > 'h')
        return 0;
    return ch - 'a' + 1;
}

int readHoris(ifstream& in)
{
    char ch;
    in >> ch;
    if (ch < '1' || ch > '8')
        return 0;
    return ch - '1' + 1;
}

bool readQueens(const char* fileName, int* qx, int* qy)
{
    ifstream in(fileName);
    if (!in.is_open())
        return false;

    for (int i = 0; i < 8; i++)
    {
        qx[i] = readVert(in);
        qy[i] = readHoris(in);
        if (in.fail() || qx[i] == 0 || qy[i] == 0)
            return false;
    }

    return true;
}

bool isThreaten(int qx1, int qy1, int qx2, int qy2, const int* qx, const int* qy, int n)
{
    // Проверка на прямую угрозу: вертикаль, горизонталь, диагональ
    if ((qx1 == qx2) || (qy1 == qy2) || (abs(qx1 - qx2) == abs(qy1 - qy2)))
    {
        // Определяем шаги для движения от первого ферзя ко второму
        int dx = 0;
        int dy = 0;

        // Определяем направление по вертикали
        if (qx2 > qx1)
            dx = 1;
        else if (qx2 < qx1)
            dx = -1;

        // Определяем направление по горизонтали
        if (qy2 > qy1)
            dy = 1;
        else if (qy2 < qy1)
            dy = -1;

        // Проверяем клетки между двумя ферзями
        int cx = qx1 + dx;
        int cy = qy1 + dy;

        while (cx != qx2 || cy != qy2) // Пока не дошли до второго ферзя
        {
            for (int i = 0; i < n; i++)
            {
                if (qx[i] == cx && qy[i] == cy)
                    return false; // Если ферзь найден между, угроза блокируется
            }
            cx += dx; // Переходим к следующей клетке
            cy += dy;
        }

        return true; // Если между ферзями никого нет, угроза остаётся
    }

    return false; // Ферзи не угрожают друг другу
}

void writePair(ofstream& out, int qx1, int qy1, int qx2, int qy2)
{
    char qv1 = (qx1 - 1) + 'a';
    char qv2 = (qx2 - 1) + 'a';
    out << qv1 << qy1 << '-' << qv2 << qy2 << endl;
}
