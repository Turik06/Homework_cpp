#include "array.h"
#include <iostream>
#include <ctime>

using namespace std;

int flaviy(int n, int k);

int main() {
    setlocale(LC_ALL, "Russian");

    int n, k;
    cout << "Введите N и K: ";
    cin >> n >> k;

    clock_t start = clock();  // Засекаем время
    int result = flaviy(n, k);
    clock_t stop = clock();   // Конец измерения времени

    double duration = (double)(stop - start) / CLOCKS_PER_SEC;  // Время в секундах

    cout << "Выживший: " << result << endl;
    cout << "Время выполнения: " << duration << " сек" << endl;

    return 0;
}

int flaviy(int n, int k) {
    Array arr(n);

    // Заполняем массив числами от 1 до N
    for (int i = 1; i <= n; i++) {
        arr.insert(i);
    }

    int cor_index = 0;

    // Удаляем элементы с шагом k, пока не останется один
    while (arr.getSize() > 1) {
        cor_index = (cor_index + k - 1) % arr.getSize();
        arr.remove(cor_index);
    }

    return arr[0];  // Последний оставшийся элемент
}
