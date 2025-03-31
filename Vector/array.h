#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>

const int DEFAULT_CAPACITY = 10;

class ArrayException {
public:
    ArrayException() {}
    ~ArrayException() {}
   
};


class Array {
private:
    int* ptr;
    int size;
    int capacity;

public:
    explicit Array(int startCapacity = DEFAULT_CAPACITY);
    ~Array();
    Array(const Array& arr);
    Array& operator=(const Array& arr);

    int& operator[](int index);
    int get(int index) const;
    void set(int index, int elem);

    void insert(int index, int elem);
    void insert_memcpy(int index, int elem);
    void insert(int elem);

    void increaseCapacity(int newCapacity);
    void remove(int index);

    int getSize() const;

    friend std::ostream& operator<<(std::ostream& out, const Array& arr);
};

#endif