#include "Currency.h"
#include "Purse.h"
#include <iostream>

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    Purse purse;
    while (true) {
        cout << "\n1. Показать кошелек\n";
        cout << "2. История пополнений\n";
        cout << "3. Добавить валюту\n";
        cout << "4. Случайно наполнить\n";
        cout << "0. Выйти\n";
        cout << "Выберите действие: ";
        int cmd;
        cin >> cmd;
        if (cmd == 0) break;
        if (cmd == 1) {
            purse.print();
        }
        else if (cmd == 2) {
            purse.printHistory();
        }
        else if (cmd == 3) {
            cout << "1. Доллар 2. Евро 3. Фунт: ";
            int t; cin >> t;
            cout << "Сумма: ";
            double sum; cin >> sum;
            cout << "Курс к рублю: ";
            double rate; cin >> rate;
            if (t == 1) {
                Dollar d(sum, rate);
                purse.addCurrency(d);
            }
            else if (t == 2) {
                Euro e(sum, rate);
                purse.addCurrency(e);
            }
            else if (t == 3) {
                Pound p(sum, rate);
                purse.addCurrency(p);
            }
            else {
                cout << "Ошибка: нет такой валюты\n";
            }
        }
        else if (cmd == 4) {
            cout << "Сколько валют добавить? ";
            int n; cin >> n;
            purse.fillRandomly(n);
        }
        else {
            cout << "Нет такой команды\n";
        }
    }
    cout << "Пока!\n";
    return 0;
}
