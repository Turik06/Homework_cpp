#include "array.h" 
#include <iostream>

using namespace std;

Array::Array(int startCapacity) : size(0) {
    capacity = (startCapacity > 0) ? startCapacity : DEFAULT_CAPACITY;
    ptr = new int[capacity];
}


Array::~Array() {
    delete[] ptr;
}


Array::Array(const Array& arr) : size(arr.size), capacity(arr.capacity) {
    ptr = new int[capacity];
    for (int i = 0; i < size; ++i) {
        ptr[i] = arr.ptr[i];
    }
}


Array& Array::operator=(const Array& arr) {
    if (this == &arr) {
        return *this;
    }

    delete[] ptr;

    size = arr.size;
    capacity = arr.capacity;
    ptr = new int[capacity];
    for (int i = 0; i < size; ++i) {
        ptr[i] = arr.ptr[i];
    }
    return *this;
}


int& Array::operator[](int index) {
    if (index < 0 || index >= size) {
        throw ArrayException();
    }
    return ptr[index];
}

int Array::get(int index) const {
    if (index < 0 || index >= size) {
        throw ArrayException(); 
    }
    return ptr[index];
}


void Array::set(int index, int elem) {
    if (index < 0 || index >= size) {
        throw ArrayException(); 
    }
    ptr[index] = elem;
}

void Array::insert(int index, int elem) {
    if (index < 0 || index > size) {
        throw ArrayException(); 
    }

    if (size == capacity) {
        increaseCapacity(size + 1);
    }

    for (int i = size; i > index; --i) {
        ptr[i] = ptr[i - 1];
    }

    ptr[index] = elem;
    size++;
}

void Array::insert_memcpy(int index, int elem) {
    if (index < 0 || index > size)
        throw ArrayException();

    if (size == capacity)
        increaseCapacity(size + 1);

    if (index < size)
        memcpy(ptr + index + 1, ptr + index, (size - index) * sizeof(int));
}


void Array::insert(int elem) {
    insert(size, elem);
}


void Array::increaseCapacity(int newCapacity) {
    if (newCapacity <= capacity) {
        return;
    }

    int newCapacityValue = (newCapacity < capacity * 2) ? capacity * 2 : newCapacity;
    int* newPtr = new int[newCapacityValue];

    for (int i = 0; i < size; ++i) {
        newPtr[i] = ptr[i];
    }

    delete[] ptr;
    ptr = newPtr;
    capacity = newCapacityValue;
}

void Array::remove(int index) {
    if (index < 0 || index >= size) {
        throw ArrayException();
    }

    for (int i = index; i < size - 1; ++i) {
        ptr[i] = ptr[i + 1];
    }

    size--;
}

int Array::getSize() const {
    return size;
}

ostream& operator<<(ostream& out, const Array& arr) {
    out << "Array size: " << arr.size << ", capacity: " << arr.capacity << " [ ";
    for (int i = 0; i < arr.size; ++i) {
        out << arr.ptr[i] << " ";
    }
    out << "]";
    return out;
}