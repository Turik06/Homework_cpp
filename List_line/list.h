#ifndef LINELIST_H
#define LINELIST_H

#include <iostream>
#include <stdexcept>

using namespace std;

template <class T> class LineList;

template <class T> class LineListElem {
    T data;
    LineListElem* next;
public:
    LineListElem(const T& adata, LineListElem* anext);
    const T& getData() const;
    LineListElem* getNext();
    template <class U> friend class LineList;
};

template <class T>
class LineList {
    LineListElem<T>* start;

    // Приватный оператор присваивания
    LineList<T>& operator =(const LineList& list);

public:
    LineList();
    LineList(const LineList& list);
    ~LineList();
    LineListElem<T>* getStart();
    void deleteFirst();
    void deleteAfter(LineListElem<T>* ptr);
    void insertFirst(const T& data);
    void insertAfter(LineListElem<T>* ptr, const T& data);

    // Дружественная функция для операторов присваивания
    friend void assignList(LineList<T>& dest, const LineList<T>& src) {
        dest = src;  // Мы можем безопасно использовать приватный оператор присваивания
    }

    template <class U> friend ostream& operator <<(ostream& out, LineList<U>& list);
};

template <class T>
LineListElem<T>::LineListElem(const T& adata, LineListElem<T>* anext) : data(adata), next(anext) {}

template <class T>
const T& LineListElem<T>::getData() const {
    return data;
}

template <class T>
LineListElem<T>* LineListElem<T>::getNext() {
    return next;
}

template <class T>
LineList<T>::LineList() : start(nullptr) {}

template <class T>
LineList<T>::LineList(const LineList& list) : start(nullptr) {
    LineListElem<T>* current_other = list.start;
    LineListElem<T>** ptr_to_next = &start;

    while (current_other) {
        LineListElem<T>* new_elem = new LineListElem<T>(current_other->data, nullptr);
        *ptr_to_next = new_elem;
        ptr_to_next = &(new_elem->next);
        current_other = current_other->next;
    }
}

template <class T>
LineList<T>::~LineList() {
    while (start) {
        deleteFirst();
    }
}

template <class T>
LineListElem<T>* LineList<T>::getStart() {
    return start;
}

template <class T>
void LineList<T>::deleteFirst() {
    if (start) {
        LineListElem<T>* temp = start->next;
        delete start;
        start = temp;
    }
    else {
        throw runtime_error("LineList is empty");
    }
}

template <class T>
void LineList<T>::deleteAfter(LineListElem<T>* ptr) {
    if (ptr && ptr->next) {
        LineListElem<T>* temp = ptr->next;
        ptr->next = ptr->next->next;
        delete temp;
    }
    else {
        throw runtime_error("Invalid pointer or no element after ptr");
    }
}

template <class T>
void LineList<T>::insertFirst(const T& data) {
    LineListElem<T>* second = start;
    start = new LineListElem<T>(data, second);
}

template <class T>
void LineList<T>::insertAfter(LineListElem<T>* ptr, const T& data) {
    if (ptr) {
        LineListElem<T>* temp = ptr->next;
        ptr->next = new LineListElem<T>(data, temp);
    }
}

template <class T>
ostream& operator <<(ostream& out, LineList<T>& list) {
    LineListElem<T>* ptr = list.start;
    if (!ptr) {
        out << "EMPTY ";
    }
    else {
        while (ptr) {
            out << ptr->getData() << ' ';
            ptr = ptr->getNext();
        }
    }
    return out;
}

template <class T>
LineList<T>& LineList<T>::operator =(const LineList& list) {
    if (this == &list) {
        return *this;
    }

    while (start) {
        deleteFirst();
    }

    LineListElem<T>* current_other = list.start;
    LineListElem<T>** ptr_to_next = &start;

    while (current_other) {
        LineListElem<T>* new_elem = new LineListElem<T>(current_other->data, nullptr);
        *ptr_to_next = new_elem;
        ptr_to_next = &(new_elem->next);
        current_other = current_other->next;
    }

    return *this;
}

#endif
