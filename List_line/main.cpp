#include "list.h"
#include<iostream>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");


    LineList<int> list1;
    list1.insertFirst(1);
    list1.insertFirst(2);
    list1.insertFirst(3);
    cout << "list1 после добавления 3 элементов: " << list1 << endl;

    LineList<int> list2(list1);
    cout << "list2 после конструктора копирования: " << list2 << endl;

    list1.deleteFirst();
    cout << "list1 после удаления первого элемента: " << list1 << endl;

    LineListElem<int>* ptr = list1.getStart();
    if (ptr) {
        list1.insertAfter(ptr, 4);
    }
    cout << "list1 после вставки элемента после первого: " << list1 << endl;

    if (ptr && ptr->getNext()) {
        list1.deleteAfter(ptr);
    }
    cout << "list1 после удаления элемента после первого: " << list1 << endl;

    LineList<int> list3;
    list3.insertFirst(5);
    list3.insertFirst(6);
    cout << "list3 до присваивания: " << list3 << endl;

    assignList(list3, list1);
    cout << "list3 после присваивания от list1: " << list3 << endl;

    cout << "list1 с использованием оператора <<: " << list1 << endl;

    LineList<int> emptyList;
    cout << "emptyList (должен быть пустым): " << emptyList << endl;

    try {
        emptyList.deleteFirst();
    }
    catch (const runtime_error& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }

    return 0;
}
