#include "array.h"
#include <iostream>
#include <ctime>

using namespace std;

int flaviy(int n, int k);

int main() {
    setlocale(LC_ALL, "Russian");

    int n, k;
    cout << "������� N � K: ";
    cin >> n >> k;

    clock_t start = clock();  // �������� �����
    int result = flaviy(n, k);
    clock_t stop = clock();   // ����� ��������� �������

    double duration = (double)(stop - start) / CLOCKS_PER_SEC;  // ����� � ��������

    cout << "��������: " << result << endl;
    cout << "����� ����������: " << duration << " ���" << endl;

    return 0;
}

int flaviy(int n, int k) {
    Array arr(n);

    // ��������� ������ ������� �� 1 �� N
    for (int i = 1; i <= n; i++) {
        arr.insert(i);
    }

    int cor_index = 0;

    // ������� �������� � ����� k, ���� �� ��������� ����
    while (arr.getSize() > 1) {
        cor_index = (cor_index + k - 1) % arr.getSize();
        arr.remove(cor_index);
    }

    return arr[0];  // ��������� ���������� �������
}
