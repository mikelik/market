#include "DividendStrategy.h"
#include <stdexcept>
#include "Stock.h"

using namespace std;
DividendStrategy::DividendStrategy(Stock* aStock)
{
	if (NULL == aStock)
		throw invalid_argument("NULL pointer to stock");
}

CommonDividend::CommonDividend(Stock* aStock) : DividendStrategy(aStock),
lastDividend(aStock->GetLastDividend())
{
}

Price CommonDividend::Calculate(const Price& tickerPrice) const
{
	return lastDividend / tickerPrice;
}

PreferredDividend::PreferredDividend(Stock* aStock) : DividendStrategy(aStock),
parValue(aStock->GetParValue()), fixedDividend(aStock->GetFixedDividend())
{
}


Price PreferredDividend::Calculate(const Price& tickerPrice) const
{
	return fixedDividend * parValue / tickerPrice;
}

