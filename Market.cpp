#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>
#include <assert.h>
#include <chrono>


#include "DividendStrategy.h"
#include "Stock.h"
#include "Trade.h"

using namespace std;

class Market {
public:
	typedef pair<unordered_map<string, Stock>::const_iterator, bool> StockInsertionPair;

	typedef unordered_map<string, deque<Trade>> TradesMap;
	typedef unordered_map<string, deque<Trade>>::const_iterator TradeConstIterator;
	typedef pair<unordered_map<string, deque<Trade>>::const_iterator, bool> TradeInsertionPair;

	Price GetStockPrice(const string& symbol) const;
	Price GetTickerPrice(const string& symbol) const { return tickerPrices.at(symbol); }
	Price GetDividend(const string& symbol) const { return stocks.at(symbol).GetDividendYield(GetTickerPrice(symbol)); };
	Price GetPERatio(const string& symbol) const { return GetTickerPrice(symbol) / GetDividend(symbol); }
	Price GetIndex();

	bool AddStock(const std::string& symbol, Price parValue, Stock::DividendType dividendType, double dividend)
	{
		return stocks.insert(make_pair(symbol, Stock(symbol, parValue, dividendType, dividend))).second;
	}

	size_t GetStocksSize() const
	{
		return stocks.size();
	}

	unsigned long GetPosition(const std::string& symbol) const //noexcept
	{
		unsigned int position = 0;
		if (positions.find(symbol) != positions.end())
			position = positions.at(symbol);
		return position;
	}

	bool AddTrade(const std::string& symbol, unsigned int quantity, Price price, Trade::TradeType type)
	{
		//Handle position first
		unsigned int position = GetPosition(symbol);
		if (Trade::TradeType::SELL == type && quantity > position)
			return false; //cannot have negative position
		if (Trade::TradeType::BUY == type && position > numeric_limits<unsigned long>::max() - quantity)
			return false; //overflow
		
		bool tradeResult = HandleTrade(symbol, quantity, price, type);
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

private:
	bool HandleTrade(const std::string& symbol, unsigned int quantity, Price price, Trade::TradeType type)
	{
		TradeConstIterator symbolIt = trades.find(symbol);

		//add trade
		if (symbolIt == trades.cend())
		{
			return trades.insert(make_pair(symbol, deque<Trade> {Trade(symbol, quantity, price, type)})).second;
		}

		trades[symbol].push_back(Trade(symbol, quantity, price, type));
		return true;
	}

	unordered_map<string, Stock> stocks; //symbol to stock
	unordered_map<string, Price> tickerPrices; //symbol to ticker price
	TradesMap trades; //symbol to trades
	unordered_map<string, unsigned long> positions; //symbol to position
};

Price Market::GetStockPrice(const string& symbol) const
{
	static const std::chrono::minutes FIFTEEN_MINS(15);
	const Timestamp NOW = std::chrono::system_clock::now();
	Price sum = 0;
	unsigned int quantity = 0;
	auto itTrade = trades.at(symbol);
	for (auto it = itTrade.cbegin(); it != itTrade.cend(); ++it)
	{
		if (std::chrono::duration_cast <std::chrono::minutes>(NOW - it->GetTimestamp()) > FIFTEEN_MINS)
			break;

		sum += it->GetPrice() * it->GetQuantity();  //overflow not handled
		quantity += it->GetQuantity();
	}

	return sum / quantity;

}

int main(int argc, char* argv[])
{
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
	assert(market.AddTrade("TEA", numeric_limits<unsigned long>::max(), 95, Trade::TradeType::BUY) == false); //overflow
	assert(market.GetPosition("TEA") == 5+12);
	const Price EPS = numeric_limits<Price>::epsilon();
	cout.precision(10);

	assert(market.GetStockPrice("TEA") == (105.0 * 5 +  95 * 12) / (5 + 12));

	market.AddTrade("TEA", 16, 99, Trade::TradeType::SELL);
	assert(market.GetPosition("TEA") == 1);
	market.AddTrade("POP", 1, 90, Trade::TradeType::BUY);
	market.AddTrade("POP", 2, 100, Trade::TradeType::BUY);
	market.AddTrade("POP", 3, 80, Trade::TradeType::SELL);
	assert(market.GetPosition("POP") == 0);

	assert(market.GetPosition("JOE") == 0);
	assert(market.GetPosition("XYZ") == 0);


	assert(market.GetDividend("TEA") <= EPS);
	assert(market.GetPERatio("TEA") == numeric_limits<Price>::infinity());
	assert(market.GetDividend("POP") == 8/80.0);
	assert(market.GetPERatio("POP") == 80/(8/80.0));

	market.AddTrade("ALE", 10, 46, Trade::TradeType::BUY);
	assert(market.GetDividend("ALE") - (23/46.0) <= EPS);
	assert(market.GetPERatio("ALE") == 46/(23/46.0));

	market.AddTrade("GIN", 1, 200, Trade::TradeType::BUY);

	assert(market.GetDividend("GIN") - (0.02*100 / 200) <= EPS);
	assert(market.GetPERatio("GIN") - 200 / ((0.02 * 100 / 200)) <= EPS);



		int x;
	cin >> x;
	return 0;
}

