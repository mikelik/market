#include "DividendStrategy.h"
#include <exception>
#include "Stock.h"

DividendStrategy::DividendStrategy(Stock* aStock)
{
	if (NULL == aStock)
		throw std::invalid_argument("NULL pointer to stock");
};

CommonDividend::CommonDividend(Stock* aStock) : DividendStrategy(aStock),
lastDividend(aStock->GetLastDividend())
{
}

Price CommonDividend::Calculate(const Price& tickerPrice) const
{
	return lastDividend / tickerPrice;
}

PreferredDividend::PreferredDividend(Stock* aStock) : DividendStrategy(aStock),
fixedDividend(aStock->GetFixedDividend()), parValue(aStock->GetParValue())
{
}


Price PreferredDividend::Calculate(const Price& tickerPrice) const
{
	return fixedDividend * parValue / tickerPrice;
}

