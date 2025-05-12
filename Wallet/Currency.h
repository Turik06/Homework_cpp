#pragma once
#ifndef CURRENCY_H
#define CURRENCY_H

#include <iostream>
#include "DateTime.h"
using namespace std;

// ����������� �����
class Currency {
protected:
    double amount;
    double exchangeRate; // ���� � �����
    DateTime operationDate; // ���� ��������

public:
    Currency(double amount, double exchangeRate);
    virtual ~Currency() = default;

    virtual double toRubles() const = 0; // ������� � �����
    virtual void display() const = 0; // ����� �� �����

 
    double getAmount() const;
    void setAmount(double amount);
    double getExchangeRate() const;
    void setExchangeRate(double rate);
    DateTime getOperationDate() const;

 
    virtual Currency* clone() const = 0;
};

// ����� ��� �������
class Dollar : public Currency {
public:
    Dollar(double amount, double exchangeRate);

    double toRubles() const override;
    void display() const override;
    double toCents() const; // ������� � �����

    Currency* clone() const override;
};

// ����� ��� ����
class Euro : public Currency {
public:
    Euro(double amount, double exchangeRate);

    double toRubles() const override;
    void display() const override;
    double toCents() const; // ������� � �����

    Currency* clone() const override;
};

// ����� ��� ����� ����������
class Pound : public Currency {
public:
    Pound(double amount, double exchangeRate);

    double toRubles() const override;
    void display() const override;
    double toPence() const; // ������� � �����

    Currency* clone() const override;
};

#endif
