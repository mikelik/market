#include "Stock.h"
#include "DividendStrategy.h"
#include <stdexcept>

using namespace std;

Stock::Stock(string aSymbol, Price aParValue, DividendType aDividendType, double aDividend) :
symbol(aSymbol), parValue(aParValue), fixedDividend(0.0), lastDividend(0.0), dividendType(aDividendType)
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
		throw invalid_argument("Dividend type not implemented.");
	}
}

Stock::Stock(Stock&& s)
{
	symbol = move(s.symbol);
	parValue = s.parValue;
	dividendType = s.dividendType;
	fixedDividend = s.fixedDividend;
	lastDividend = s.lastDividend;
	dividendStrategy = move(s.dividendStrategy);
}
Stock& Stock::operator=(Stock&& s)
{
	if (this != &s)
	{
		symbol = move(s.symbol);
		parValue = s.parValue;
		dividendType = s.dividendType;
		fixedDividend = s.fixedDividend;
		lastDividend = s.lastDividend;
		dividendStrategy = move(s.dividendStrategy);
	}

	return *this;
}
Price Stock::GetDividendYield(const Price& tickerPrice) const
{
	return dividendStrategy->Calculate(tickerPrice);
}