#pragma once
#ifndef PURSE_H
#define PURSE_H

#include "Currency.h"
#include <vector>

class Purse {
private:
    // ��������� ��� ������ ������� ���������� ��������
    struct HistoryRecord {
        Currency* currency;
        DateTime dateTime;

        HistoryRecord(Currency* curr, const DateTime& dt);
        ~HistoryRecord();
    };

    std::vector<Currency*> currencies; // ������ � ��������
    std::vector<HistoryRecord*> history; // ������� ����������

public:
    Purse();
    ~Purse();

    // ��������� ����������� � ������������ 
    Purse(const Purse&) = delete;
    Purse& operator=(const Purse&) = delete;

    void addCurrency(const Currency& currency); // �������� ������ � �������
    void fillRandomly(int count); // ��������� ���������� ��������
    double getTotalInRubles() const; // ����� ����� � ������
    void print() const; // ����� ���������� � ��������
    void printHistory() const; // ����� ������� ����������

    size_t getSize() const { return currencies.size(); }
    Currency* getCurrency(size_t index) const;
};

#endif
