#include <iostream>

#include <assert.h>
#include <chrono>
#include <cmath>

#include "Market.h"

using namespace std;

bool Market::AddTrade(const std::string& symbol, unsigned int quantity, Price price, Trade::TradeType type, const Timestamp& timestamp /*= std::chrono::system_clock::now()*/)
{
	//Handle position first
	unsigned int position = GetPosition(symbol);
	if (Trade::TradeType::SELL == type && quantity > position)
		return false; //cannot have negative position
	if (Trade::TradeType::BUY == type && position > numeric_limits<unsigned int>::max() - quantity)
		return false; //overflow

	bool tradeResult = HandleTrade(symbol, quantity, price, type, timestamp);
	if (tradeResult)
	{
		tickerPrices[symbol] = price;
		if (Trade::TradeType::BUY == type)
			positions[symbol] = position + quantity;
		else
			positions[symbol] = position - quantity;
	}
	return tradeResult;
}

bool Market::HandleTrade(const std::string& symbol, unsigned int quantity, Price price, Trade::TradeType type, const Timestamp& timestamp)
{
	TradeConstIterator symbolIt = trades.find(symbol);

	//add trade
	if (symbolIt == trades.cend())
	{
		return trades.insert(std::make_pair(symbol, std::deque<Trade> {Trade(symbol, quantity, price, type, timestamp)})).second;
	}

	trades[symbol].push_back(Trade(symbol, quantity, price, type));
	return true;
}


Price Market::GetStockPrice(const string& symbol) const
{
	static const std::chrono::minutes FIFTEEN_MINS(15);
	const Timestamp NOW = std::chrono::system_clock::now();
	Price sum = 0;
	unsigned int quantity = 0;
	auto itTrade = trades.at(symbol);
	for (auto it = itTrade.crbegin(); it != itTrade.crend(); ++it)
	{
		if (std::chrono::duration_cast <std::chrono::minutes>(NOW - it->GetTimestamp()) > FIFTEEN_MINS)
			break;

		sum += it->GetPrice() * it->GetQuantity();  //overflow not handled
		quantity += it->GetQuantity();
	}

	return sum / quantity;

}

Price Market::GetIndex() const
{
	Price index = 1;
	const size_t size = tickerPrices.size();
	for (const auto& it : tickerPrices)
	{
		index *= pow(it.second, 1.0/size);
	}

	return index;
}

int main(int argc, char* argv[])
{
	const long double EPS = 0.000001;
	Market market;
	assert(market.AddStock("TEA", 100, Stock::DividendType::COMMON, 0));
	
	assert(market.AddStock("TEA", 105, Stock::DividendType::COMMON, 0) == false);
	market.AddStock("POP", 100, Stock::DividendType::COMMON, 8);
	market.AddStock("ALE", 60, Stock::DividendType::COMMON, 23);
	market.AddStock("GIN", 100, Stock::DividendType::PREFERRED, 0.02);
	market.AddStock("JOE", 250, Stock::DividendType::COMMON, 13);
	assert(market.GetStocksSize() == 5);

	assert(market.AddTrade("TEA", 1, 105, Trade::TradeType::SELL) == false); //cannot have negative position
	assert(market.AddTrade("TEA", 5, 105, Trade::TradeType::BUY));
	assert(market.AddTrade("TEA", 7, 105, Trade::TradeType::SELL) == false); //cannot have negative position
	assert(market.AddTrade("TEA", 12, 95, Trade::TradeType::BUY));
	assert(market.AddTrade("TEA", numeric_limits<unsigned int>::max(), 95, Trade::TradeType::BUY) == false); //overflow
	assert(market.GetPosition("TEA") == 5+12);
	cout.precision(10);

	assert(fabs(market.GetStockPrice("TEA") - (105.0 * 5 + ( 95 * 12)) / (5 + 12)) <= EPS);

	market.AddTrade("TEA", 16, 99, Trade::TradeType::SELL);
	assert(market.GetPosition("TEA") == 1);
	market.AddTrade("POP", 1, 90, Trade::TradeType::BUY);
	market.AddTrade("POP", 2, 100, Trade::TradeType::BUY);
	market.AddTrade("POP", 3, 80, Trade::TradeType::SELL);
	assert(market.GetPosition("POP") == 0);

	assert(market.GetPosition("JOE") == 0);
	assert(market.GetPosition("XYZ") == 0);


	assert(fabs(market.GetDividend("TEA")) <= EPS);
	assert(market.GetPERatio("TEA") == numeric_limits<Price>::infinity());
	assert(fabs(market.GetDividend("POP") - 8/80.0) < EPS);
	assert(market.GetPERatio("POP") == 80/(8/80.0));

	market.AddTrade("ALE", 10, 46, Trade::TradeType::BUY);
	assert(fabs(market.GetDividend("ALE") - (23/46.0)) <= EPS);
	assert(market.GetPERatio("ALE") == 46/(23/46.0));

	market.AddTrade("GIN", 1, 200, Trade::TradeType::BUY);

	assert(fabs(market.GetDividend("GIN") - (0.02*100 / 200)) <= EPS);
	assert(fabs(market.GetPERatio("GIN") - 200 / ((0.02 * 100 / 200))) <= EPS);


	assert(market.AddTrade("JOE", 1, 105, Trade::TradeType::BUY, std::chrono::system_clock::now() - std::chrono::minutes(16)));
	assert(market.AddTrade("JOE", 5, 105, Trade::TradeType::BUY, std::chrono::system_clock::now() - std::chrono::minutes(14)));
	assert(market.AddTrade("JOE", 7, 105, Trade::TradeType::BUY, std::chrono::system_clock::now() - std::chrono::minutes(3)));
	assert(market.AddTrade("JOE", 12, 95, Trade::TradeType::BUY));
	assert(market.AddTrade("JOE", 1, 95, Trade::TradeType::SELL));
	assert(market.GetPosition("JOE") == 5 + 7 + 12);

	assert(fabs(market.GetStockPrice("JOE") - (5*105.0+7*105+12*95+95) / (5 +7+1+ 12)) < EPS);
	
	assert(fabs(market.GetIndex() - pow(99.0 * 80 * 46 * 200 * 95, 1 / 5.0)) < EPS);

	return 0;
}

