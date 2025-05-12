#include "Currency.h"
#include <iomanip>

// Константы для перевода в дробные единицы
const int CENTS_PER_DOLLAR = 100;
const int CENTS_PER_EURO = 100;
const int PENCE_PER_POUND = 100;

// Реализация методов абстрактного класса Currency
Currency::Currency(double amount, double exchangeRate)
    : amount(amount), exchangeRate(exchangeRate), operationDate() {}

double Currency::getAmount() const {
    return amount;
}

void Currency::setAmount(double amount) {
    this->amount = amount;
}

double Currency::getExchangeRate() const {
    return exchangeRate;
}

void Currency::setExchangeRate(double rate) {
    exchangeRate = rate;
}

DateTime Currency::getOperationDate() const {
    return operationDate;
}

// Реализация класса Dollar
Dollar::Dollar(double amount, double exchangeRate)
    : Currency(amount, exchangeRate) {}

double Dollar::toRubles() const {
    return amount * exchangeRate;
}

void Dollar::display() const {
    cout << fixed << setprecision(2) << amount << " USD (курс: "
        << exchangeRate << " руб. за 1 USD, "
        << toRubles() << " руб.)" << endl;
}

double Dollar::toCents() const {
    return amount * CENTS_PER_DOLLAR;
}

Currency* Dollar::clone() const {
    return new Dollar(*this);
}

// Реализация класса Euro
Euro::Euro(double amount, double exchangeRate)
    : Currency(amount, exchangeRate) {}

double Euro::toRubles() const {
    return amount * exchangeRate;
}

void Euro::display() const {
    cout << fixed << setprecision(2) << amount << " EUR (курс: "
        << exchangeRate << " руб. за 1 EUR, "
        << toRubles() << " руб.)" << endl;
}

double Euro::toCents() const {
    return amount * CENTS_PER_EURO;
}

Currency* Euro::clone() const {
    return new Euro(*this);
}

// Реализация класса Pound
Pound::Pound(double amount, double exchangeRate)
    : Currency(amount, exchangeRate) {}

double Pound::toRubles() const {
    return amount * exchangeRate;
}

void Pound::display() const {
    cout << fixed << setprecision(2) << amount << " GBP (курс: "
        << exchangeRate << " руб. за 1 GBP, "
        << toRubles() << " руб.)" << endl;
}

double Pound::toPence() const {
    return amount * PENCE_PER_POUND;
}

Currency* Pound::clone() const {
    return new Pound(*this);
}
