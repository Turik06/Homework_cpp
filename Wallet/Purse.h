#pragma once
#ifndef PURSE_H
#define PURSE_H

#include "Currency.h"
#include <vector>

class Purse {
private:
    // Структура для записи истории пополнения кошелька
    struct HistoryRecord {
        Currency* currency;
        DateTime dateTime;

        HistoryRecord(Currency* curr, const DateTime& dt);
        ~HistoryRecord();
    };

    std::vector<Currency*> currencies; // Валюты в кошельке
    std::vector<HistoryRecord*> history; // История пополнения

public:
    Purse();
    ~Purse();

    // Запрещаем копирование и присваивание 
    Purse(const Purse&) = delete;
    Purse& operator=(const Purse&) = delete;

    void addCurrency(const Currency& currency); // Добавить валюту в кошелек
    void fillRandomly(int count); // Наполнить случайными валютами
    double getTotalInRubles() const; // Общая сумма в рублях
    void print() const; // Вывод информации о кошельке
    void printHistory() const; // Вывод истории пополнения

    size_t getSize() const { return currencies.size(); }
    Currency* getCurrency(size_t index) const;
};

#endif
