#pragma once

#include <memory>
class Stock;

typedef long double Price;

class DividendStrategy {
public:
	explicit DividendStrategy(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const = 0;
};

class CommonDividend : public DividendStrategy {
public:
	explicit CommonDividend(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const;
protected:
	double lastDividend;
};

class PreferredDividend : public DividendStrategy {
public:
	explicit PreferredDividend(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const;
protected:
	Price parValue;
	double fixedDividend;
};

