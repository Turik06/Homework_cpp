#include "Datetime.h"
#include <iostream>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    try {
        DateTime dt1;
        cout << "Введите дату и время (год месяц день часы минуты секунды): ";
        cin >> dt1;

        cout << "Оригинальная дата и время:\n";
        dt1.print();
        dt1.printISO();
        cout << "День недели: " << dt1.getDayOfWeek() << endl;

        int daysToAdd;
        cout << "Введите количество дней для добавления: ";
        cin >> daysToAdd;
        DateTime dt2 = dt1 + daysToAdd;
        cout << "Дата после добавления " << daysToAdd << " дней:\n" << dt2;

        int daysToSubtract;
        cout << "Введите количество дней для вычитания: ";
        cin >> daysToSubtract;
        dt1 -= daysToSubtract;
        cout << "Дата после вычитания " << daysToSubtract << " дней:\n" << dt1;


        
        DateTime dt3;
        cout << "Введите дату и время (год месяц день часы минуты секунды): ";
        cin >> dt3;
        int diff = dt3.differenceInDays(dt1);
        cout << "Разница между датами в днях: " << diff << endl;

        int easterYear;
        cout << "Введите год для вычисления даты Пасхи: ";
        cin >> easterYear;
        DateTime easter = DateTime::getEasterDate(easterYear);
        cout << "Дата Пасхи в " << easterYear << " году: " << easter << endl;
    }
    catch (const DateException&) {
        cout << "Ошибка: Некорректная дата!" << endl;
    }

    return 0;
}
