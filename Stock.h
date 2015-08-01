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

private:
	std::string symbol;
	Price parValue;
	double fixedDividend;
	double lastDividend;
	DividendType dividendType;
	std::unique_ptr<DividendStrategy> dividendStrategy;

	//non copyable because of DividendStrategy
	Stock(const Stock&) = delete;
	Stock& operator=(const Stock&) = delete;
};
