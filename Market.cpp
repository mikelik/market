#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>



#include "DividendStrategy.h"
#include "Stock.h"
#include "Trade.h"

using namespace std;

class Market {
public:
	Price GetStockPrice(const string& symbol);
	Price GetTickerPrice(const string& symbol);
	Price GetDividend(const string& symbol);
	Price GetPERatio(const string& symbol);
	Price GetIndex();

	void AddStock(const std::string& symbol, Price parValue, Stock::DividendType dividendType, double dividend)
	{
		stocks.insert(make_pair(symbol, Stock(symbol, parValue, dividendType, dividend)));
	}
	void AddTrade(const std::string& symbol, unsigned int quantity, Price price, Trade::TradeType type)
	{
		if (trades.find(symbol) == trades.cend())
		{
			trades.insert(make_pair(symbol, deque<Trade> {Trade(symbol, quantity, price, type)}));

		}
		else
		{
			trades[symbol].push_back(Trade(symbol, quantity, price, type));
		}
		//
	}

private:

	unordered_map<string, Stock> stocks; //symbol to stock
	unordered_map<string, Price> tickerPrices; //symbol to ticker price
	unordered_map<string, deque<Trade>> trades; //symbol to trades
	unordered_map<string, unsigned long> positions; //symbol to position
};


int main(int argc, char* argv[])
{
	Market market;
	market.AddStock("TEA", 100, Stock::DividendType::COMMON, 0);
	market.AddStock("POP", 100, Stock::DividendType::COMMON, 8);
	market.AddStock("ALE", 60, Stock::DividendType::COMMON, 23);
	market.AddStock("GIN", 100, Stock::DividendType::PREFERRED, 0.02);
	market.AddStock("JOE", 250, Stock::DividendType::COMMON, 13);


	market.AddTrade("TEA", 5, 105, Trade::TradeType::BUY);
	market.AddTrade("TEA", 12, 95, Trade::TradeType::BUY);
	market.AddTrade("TEA", 16, 99, Trade::TradeType::SELL);
	market.AddTrade("POP", 1, 90, Trade::TradeType::BUY);
	market.AddTrade("POP", 2, 100, Trade::TradeType::BUY);
	market.AddTrade("POP", 3, 80, Trade::TradeType::SELL);
	return 0;
}

