#include "Stock.h"
#include "DividendStrategy.h"

Stock::Stock(std::string aSymbol, Price aParValue, DividendType aDividendType, double aDividend) :
symbol(aSymbol), parValue(aParValue), dividendType(aDividendType), fixedDividend(0.0), lastDividend(0.0)
{
	if (DividendType::COMMON == aDividendType)
	{
		lastDividend = aDividend;
		dividendStrategy.reset(new CommonDividend(this));

	}
	else if (DividendType::PREFERRED == aDividendType)
	{
		fixedDividend = aDividend;
		dividendStrategy.reset(new PreferredDividend(this));
	}
	else
	{
		throw std::invalid_argument("Dividend type not implemented.");
	}
}

Price Stock::GetDividendYield(const Price& tickerPrice) const
{
	return dividendStrategy->Calculate(tickerPrice);
}