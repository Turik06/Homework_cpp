#pragma once
#ifndef CURRENCY_H
#define CURRENCY_H

#include <iostream>
#include "DateTime.h"
using namespace std;

// Абстрактный класс
class Currency {
protected:
    double amount;
    double exchangeRate; // Курс к рублю
    DateTime operationDate; // Дата операции

public:
    Currency(double amount, double exchangeRate);
    virtual ~Currency() = default;

    virtual double toRubles() const = 0; // Перевод в рубли
    virtual void display() const = 0; // Вывод на экран

 
    double getAmount() const;
    void setAmount(double amount);
    double getExchangeRate() const;
    void setExchangeRate(double rate);
    DateTime getOperationDate() const;

 
    virtual Currency* clone() const = 0;
};

// Класс для доллара
class Dollar : public Currency {
public:
    Dollar(double amount, double exchangeRate);

    double toRubles() const override;
    void display() const override;
    double toCents() const; // Перевод в центы

    Currency* clone() const override;
};

// Класс для евро
class Euro : public Currency {
public:
    Euro(double amount, double exchangeRate);

    double toRubles() const override;
    void display() const override;
    double toCents() const; // Перевод в центы

    Currency* clone() const override;
};

// Класс для фунта стерлингов
class Pound : public Currency {
public:
    Pound(double amount, double exchangeRate);

    double toRubles() const override;
    void display() const override;
    double toPence() const; // Перевод в пенсы

    Currency* clone() const override;
};

#endif
