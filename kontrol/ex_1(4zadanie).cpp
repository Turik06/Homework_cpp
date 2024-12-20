#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");

    int SIZE;

    cout << "Введите размер массива: ";
    cin >> SIZE;

    if (SIZE <= 0) {
        cout << "Размер массива должен быть больше 0!" << endl;
        return 1; 
    }

    int* arr = new int[SIZE];

    cout << "Введите элементы массива: ";
    for (int i = 0; i < SIZE; ++i) {
        cin >> arr[i];
    }

    int max_len = 0;  
    int now_len = 0;  

    for (int i = 0; i < SIZE; ++i) {
        if (arr[i] == 0) {
            now_len++; 
        }
        else {
            if (now_len > max_len) {
                max_len = now_len; 
            }
            now_len = 0; 
        }
    }

    if (now_len > max_len) {
        max_len = now_len;
    }

    cout << "Длина самой длинной последовательности нулей: " << max_len << endl;

    delete[] arr;

    return 0;
}
