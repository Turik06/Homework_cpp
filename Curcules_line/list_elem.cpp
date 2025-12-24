#include <iostream>
#include <ctime>
#include "list.h"
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    int cnt = 0, global_cnt = 0, max_num, victim;


    LineList<int> list;
    list.insertFirst(1);

    cout << "Введите максимальное количество элементов: ";
    cin >> max_num;

    cout << "Введите 'шаг выбывания': ";
    cin >> victim;

    clock_t start = clock();

    LineListElem<int>* ptr = list.getStart();
    for (int i = 2; i <= max_num; i++) 
    {
        list.insertAfter(ptr, i);
        ptr = ptr->getNext();
    }

    ptr = list.getStart();
    while (global_cnt != max_num - 1)
    {
        cnt++;
        if (cnt == victim - 1) 
        {
            list.deleteAfter(ptr);
            global_cnt++;
            cnt = 0;
        }
        ptr = ptr->getNext();
    }

    clock_t end = clock();

    double duration = double(end - start) / CLOCKS_PER_SEC;

    cout << "Время выполнения: " << duration << " секунд" << endl;
    cout << list << endl;

    return 0;
}
