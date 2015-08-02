#pragma once

#include <memory>
class Stock;

typedef long double Price;

class DividendStrategy {
public:
	DividendStrategy(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const = 0;
};

class CommonDividend : public DividendStrategy {
public:
	CommonDividend(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const;
protected:
	double lastDividend;
};

class PreferredDividend : public DividendStrategy {
public:
	PreferredDividend(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const;
protected:
	Price parValue;
	double fixedDividend;
};

