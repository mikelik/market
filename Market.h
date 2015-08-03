#pragma once

#include <string>
#include <unordered_map>
#include <deque>

#include "Stock.h"
#include "Trade.h"

class Market {
public:
	typedef std::pair<std::unordered_map<std::string, Stock>::const_iterator, bool> StockInsertionPair;

	typedef std::unordered_map<std::string, std::deque<Trade>> TradesMap;
	typedef TradesMap::const_iterator TradeConstIterator;
	typedef std::pair<TradeConstIterator, bool> TradeInsertionPair;

	Price GetStockPrice(const std::string& symbol) const;
	Price GetTickerPrice(const std::string& symbol) const { return tickerPrices.at(symbol); }
	Price GetDividend(const std::string& symbol) const { return stocks.at(symbol).GetDividendYield(GetTickerPrice(symbol)); };
	Price GetPERatio(const std::string& symbol) const { return GetTickerPrice(symbol) / GetDividend(symbol); }
	Price GetIndex() const;

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

	bool AddTrade(const std::string& symbol, unsigned int quantity, Price price, Trade::TradeType type, const Timestamp& timestamp = std::chrono::system_clock::now());
private:
	bool HandleTrade(const std::string& symbol, unsigned int quantity, Price price, Trade::TradeType type, const Timestamp& timestamp);

	std::unordered_map<std::string, Stock> stocks; //symbol to stock
	std::unordered_map<std::string, Price> tickerPrices; //symbol to ticker price
	TradesMap trades; //symbol to trades
	std::unordered_map<std::string, unsigned long> positions; //symbol to position
};