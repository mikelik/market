#pragma once

#include <memory>
class Stock;

typedef long double Price;

class DividendStrategy {
public:
	DividendStrategy(Stock* aStock);
	virtual Price Calculate(const Price& tickerPrice) const = 0;
	DividendStrategy(const DividendStrategy&) = delete;
	DividendStrategy& operator=(const DividendStrategy&) = delete;
	DividendStrategy(DividendStrategy&& ds) {stock = ds.}
	DividendStrategy& operator=(DividendStrategy&& ds) { stock = ds. }
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

