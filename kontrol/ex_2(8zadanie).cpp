#include <iostream>
using namespace std;

int main() {
    int n;

 
    cout << "Введите длину массивов: ";
    cin >> n;

    int X[n], Y[n];


    cout << "Введите элементы массива X: ";
    for (int i = 0; i < n; i++) {
        cin >> X[i];
    }

   
    cout << "Введите элементы массива Y: ";
    for (int i = 0; i < n; i++) {
        cin >> Y[i];
    }

 
    bool match = true;
    for (int i = 0; i < n; i++) {
        if (X[i] != Y[i]) {
            match = false;
            break;
        }
    }

   
    if (match) {
        cout << "ДА" << endl;
    }
    else {
        cout << "НЕТ" << endl;
    }

    return 0;
}
