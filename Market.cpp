#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>

#include "Stock.h"
#include "DividendStrategy.h"
#include "Trade.h"

using namespace std;





class Market
{
public:
	Price getStockPrice(const string& symbol);
	Price getTickerPrice(const string& symbol);
	Price getDividend(const string& symbol);
	Price getPERatio(const string& symbol);
	Price getIndex();

private:

	unordered_map<string, Stock> stockList; //symbol to stock
	unordered_map<string, Price> tickerPrice; //symbol to ticker price
	unordered_map<string, deque<Trade>> TradeList; //symbol to list of trades
};


int main(int argc, char* argv[])
{
	return 0;
}

