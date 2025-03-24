
#pragma once
#ifndef _LIST_H
#define _LIST_H
#include <iostream>
#include <fstream>
using namespace std;

class LineListException
{

};

template <class T> class LineList;

template <class T>
class LineListElem
{
    T data;
    LineListElem* next;

public:
    LineListElem(const T& adata, LineListElem* anext);
    const T& getData() const;
    LineListElem* getNext();
    friend class ::LineList<T>;
};

template <class T>
class LineList
{
    LineListElem<T>* start;
    LineListElem<T>* end;
    LineList(const LineList& list);
    LineList& operator =(const LineList& list);

public:
    LineList();
    ~LineList();
    LineListElem<T>* getStart();
    void deleteFirst();
    void deleteAfter(LineListElem<T>* ptr);
    void insertFirst(const T& data);
    void insertAfter(LineListElem<T>* ptr, const T& data);
    friend ostream& operator<<(ostream& out, LineList& list) {
        if (!list.start) {
            out << "EMPTY ";
        }
        else {
            LineListElem<T>* ptr = list.start;
            do {
                out << ptr->getData() << " ";
                ptr = ptr->getNext();
            } while (ptr != list.start);
        }
        return out;
    }
};

template <class T> LineListElem<T>::LineListElem(const T& adata, LineListElem<T>* anext)
{
    data = adata;
    next = anext;
}

template <class T> const T& LineListElem<T>::getData() const
{
    return data;
}

template <class T> LineListElem<T>* LineListElem<T>::getNext()
{
    return next;
}

template <class T> LineList<T>::LineList()
{
    start = 0;
    end = 0;
}

template <class T> LineList<T>::~LineList()
{
    while (start)
        deleteFirst();
}

template <class T> LineListElem<T>* LineList<T>::getStart()
{
    return start;
}

template <class T> void LineList<T>::deleteFirst()
{
    if (start)
    {
        LineListElem<T>* temp = start;
        if (start == end)
        {
            start = 0;
            end = 0;
        }
        else
        {
            start = start->next;
            end->next = start;
        }
        delete temp;
    }
    else
        throw LineListException();
}

template <class T> void LineList<T>::insertFirst(const T& data)
{
    LineListElem<T>* newElem = new LineListElem<T>(data, start);
    if (!start)
    {
        start = newElem;
        end = newElem;
        end->next = start;
    }
    else
    {
        start = newElem;
        end->next = start;
    }
}

template <class T>
void LineList<T>::deleteAfter(LineListElem<T>* ptr)
{
    if (ptr && ptr->next)
    {
        LineListElem<T>* temp = ptr->next;
        ptr->next = ptr->next->next;
        if (temp == end)
        {
            end = ptr;
            end->next = start;
        }
        else if (temp == start)
        {
            start = start->next;
            end->next = start;
        }
        delete temp;
    }
    else
    {
        throw LineListException();
    }
}

template <class T>
void LineList<T>::insertAfter(LineListElem<T>* ptr, const T& data)
{
    if (ptr) {
        LineListElem<T>* newElem = new LineListElem<T>(data, ptr->next);
        ptr->next = newElem;
        if (ptr == end) {
            end = newElem;
            end->next = start;
        }
    }
}

#endif