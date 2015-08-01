#pragma once

#include <string>
#include <memory>
#include "DividendStrategy.h"

typedef long double Price;

class Stock {
public:
	enum class DividendType { COMMON, PREFERRED };

	Stock(std::string aSymbol, Price aParValue, DividendType aDividendType, double aDividend) :
		symbol(aSymbol), parValue(aParValue), dividendType(aDividendType), fixedDividend(0.0), lastDividend(0.0)
	{
		if (DividendType::COMMON == aDividendType)
		{
			lastDividend = aDividend;
			dividendStrategy = std::make_unique<DividendStrategy>(new CommonDividend(*this));

		}
		else if (DividendType::PREFERRED == aDividendType)
		{
			fixedDividend = aDividend;
			dividendStrategy = std::make_unique<DividendStrategy>(new PreferredDividend(*this));
		}
		else
		{
			throw std::invalid_argument("Dividend type not implemented.");
		}
	}

	Price getDividendYield(const Price& tickerPrice)
	{
		return dividendStrategy->Calculate(tickerPrice);
	}

	Price getParValue() const { return parValue; }
	double getFixedDividend() const { return fixedDividend; }
	double getLastDividend() const { return lastDividend; }

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
