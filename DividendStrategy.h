#pragma once

#include <memory>
#include "Stock.h"

class Stock;
class DividendStrategy {
public:
	DividendStrategy(std::unique_ptr<Stock> aStock) { stock = std::move(aStock); };
	virtual Price Calculate(const Price& tickerPrice) const = 0;
protected:
	unique_ptr<Stock> stock;
};

class CommonDividend : public DividendStrategy {
public:
	CommonDividend(std::unique_ptr<Stock> aStock) : DividendStrategy(std::move(aStock)) {};
	virtual Price Calculate(const Price& tickerPrice) const
	{
		return stock->getLastDividend() / tickerPrice;
	}
};

class PreferredDividend : public DividendStrategy {
public:
	PreferredDividend(std::unique_ptr<Stock> aStock) : PreferredDividend(std::move(aStock)) {};
	virtual Price Calculate(const Price& tickerPrice) const
	{
		return stock->getFixedDividend() * stock->getParValue() / tickerPrice;
	}
};

