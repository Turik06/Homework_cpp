#include "Purse.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>

// ���������� ��������� HistoryRecord
Purse::HistoryRecord::HistoryRecord(Currency* curr, const DateTime& dt)
    : currency(curr->clone()), dateTime(dt) {}

Purse::HistoryRecord::~HistoryRecord() {
    delete currency;
}


Purse::Purse() = default;

Purse::~Purse() {
    // ����������� ������ ��� ���� �����
    for (auto& currency : currencies) {
        delete currency;
    }

    // ����������� ������ ��� ���� ������� � �������
    for (auto& record : history) {
        delete record;
    }
}

void Purse::addCurrency(const Currency& currency) {
    // ������� ����� ������ � ��������� �� � ������
    Currency* currencyCopy = currency.clone();
    currencies.push_back(currencyCopy);

    // ��������� ������ � �������
    history.push_back(new HistoryRecord(currencyCopy, DateTime()));
}

void Purse::fillRandomly(int count) {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(0)));
        seeded = true;
    }

    for (int i = 0; i < count; i++) {
        int type = rand() % 3;  // 0 - Dollar, 1 - Euro, 2 - Pound
        double amount = (rand() % 1000) / 10.0;  // ��������� ����� �� 0 �� 100 � ����� 0.1

        switch (type) {
        case 0: {
            Dollar dollar(amount, 75.0);  // ���� ������� � �����
            addCurrency(dollar);
            break;
        }
        case 1: {
            Euro euro(amount, 85.0);  // ���� ���� � �����
            addCurrency(euro);
            break;
        }
        case 2: {
            Pound pound(amount, 100.0);  // ���� ����� � �����
            addCurrency(pound);
            break;
        }
        }
    }
}

double Purse::getTotalInRubles() const {
    double total = 0.0;
    for (const auto& currency : currencies) {
        total += currency->toRubles();
    }
    return total;
}

void Purse::print() const {
    cout << "=== ���������� �������� ===\n";
    for (size_t i = 0; i < currencies.size(); i++) {
        cout << i + 1 << ". ";
        currencies[i]->display();
    }
    cout << "����� �����: " << fixed << setprecision(2) << getTotalInRubles() << " ���.\n";
}

void Purse::printHistory() const {
    cout << "=== ������� ���������� �������� ===\n";
    for (size_t i = 0; i < history.size(); i++) {
        cout << i + 1 << ". ";
        cout << "����: ";
        history[i]->dateTime.print();
        cout << "   ���������: ";
        history[i]->currency->display();
    }
}

Currency* Purse::getCurrency(size_t index) const {
    if (index < currencies.size()) {
        return currencies[index];
    }
    return nullptr;
}
