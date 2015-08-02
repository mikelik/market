#pragma once

#include <string>
#include <memory>
#include "DividendStrategy.h"

typedef long double Price;

class Stock {
public:
	enum class DividendType { COMMON, PREFERRED };

	Stock(std::string aSymbol, Price aParValue, DividendType aDividendType, double aDividend);

	Price GetDividendYield(const Price& tickerPrice);

	Price GetParValue() const { return parValue; }
	double GetFixedDividend() const { return fixedDividend; }
	double GetLastDividend() const { return lastDividend; }

	//non copyable because of DividendStrategy
	Stock(const Stock&) = delete;
	Stock& operator=(const Stock&) = delete;
	Stock(Stock&& s)
	{
		symbol = std::move(s.symbol);
		parValue = s.parValue;
		dividendType = s.dividendType;
		fixedDividend = s.fixedDividend;
		lastDividend = s.lastDividend;
		dividendStrategy = std::move(s.dividendStrategy);
	}
	Stock& operator=(Stock&& s)
	{
		symbol = std::move(s.symbol);
		parValue = s.parValue;
		dividendType = s.dividendType;
		fixedDividend = s.fixedDividend;
		lastDividend = s.lastDividend;
		dividendStrategy = std::move(s.dividendStrategy);
	}

private:
	std::string symbol;
	Price parValue;
	double fixedDividend;
	double lastDividend;
	DividendType dividendType;
	std::unique_ptr<DividendStrategy> dividendStrategy;
};
