#include "Purse.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>

// Реализация структуры HistoryRecord
Purse::HistoryRecord::HistoryRecord(Currency* curr, const DateTime& dt)
    : currency(curr->clone()), dateTime(dt) {}

Purse::HistoryRecord::~HistoryRecord() {
    delete currency;
}


Purse::Purse() = default;

Purse::~Purse() {
    // Освобождаем память для всех валют
    for (auto& currency : currencies) {
        delete currency;
    }

    // Освобождаем память для всех записей в истории
    for (auto& record : history) {
        delete record;
    }
}

void Purse::addCurrency(const Currency& currency) {
    // Создаем копию валюты и добавляем ее в массив
    Currency* currencyCopy = currency.clone();
    currencies.push_back(currencyCopy);

    // Добавляем запись в историю
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
        double amount = (rand() % 1000) / 10.0;  // Случайная сумма от 0 до 100 с шагом 0.1

        switch (type) {
        case 0: {
            Dollar dollar(amount, 75.0);  // Курс доллара к рублю
            addCurrency(dollar);
            break;
        }
        case 1: {
            Euro euro(amount, 85.0);  // Курс евро к рублю
            addCurrency(euro);
            break;
        }
        case 2: {
            Pound pound(amount, 100.0);  // Курс фунта к рублю
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
    cout << "=== Содержимое кошелька ===\n";
    for (size_t i = 0; i < currencies.size(); i++) {
        cout << i + 1 << ". ";
        currencies[i]->display();
    }
    cout << "Общая сумма: " << fixed << setprecision(2) << getTotalInRubles() << " руб.\n";
}

void Purse::printHistory() const {
    cout << "=== История пополнения кошелька ===\n";
    for (size_t i = 0; i < history.size(); i++) {
        cout << i + 1 << ". ";
        cout << "Дата: ";
        history[i]->dateTime.print();
        cout << "   Добавлено: ";
        history[i]->currency->display();
    }
}

Currency* Purse::getCurrency(size_t index) const {
    if (index < currencies.size()) {
        return currencies[index];
    }
    return nullptr;
}
