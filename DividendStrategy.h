#pragma once

#include <memory>
class Stock;

typedef long double Price;

class DividendStrategy {
public:
	DividendStrategy(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const = 0;
protected:
	Stock* stock;
};

class CommonDividend : public DividendStrategy {
public:
	CommonDividend(Stock* aStock) : DividendStrategy(aStock) {};
	virtual Price Calculate(const Price& tickerPrice) const;
};

class PreferredDividend : public DividendStrategy {
public:
	PreferredDividend(Stock* aStock) : DividendStrategy(aStock) {};
	virtual Price Calculate(const Price& tickerPrice) const;
};

