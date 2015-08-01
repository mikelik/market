#include "DividendStrategy.h"
#include <exception>
#include "Stock.h"

DividendStrategy::DividendStrategy(Stock* aStock)
{
	if (NULL == aStock)
		throw std::invalid_argument("NULL pointer to stock");
	stock = aStock;
};

	Price CommonDividend::Calculate(const Price& tickerPrice) const
	{
		return stock->GetLastDividend() / tickerPrice;
	}


	Price PreferredDividend::Calculate(const Price& tickerPrice) const
	{
		return stock->GetFixedDividend() * stock->GetParValue() / tickerPrice;
	}

