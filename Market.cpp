#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>



#include "DividendStrategy.h"
#include "Stock.h"
#include "Trade.h"

using namespace std;

class Market
{
public:
	Price GetStockPrice(const string& symbol);
	Price GetTickerPrice(const string& symbol);
	Price GetDividend(const string& symbol);
	Price GetPERatio(const string& symbol);
	Price GetIndex();

private:

	unordered_map<string, Stock> stockList; //symbol to stock
	unordered_map<string, Price> tickerPrice; //symbol to ticker price
	unordered_map<string, deque<Trade>> TradeList; //symbol to list of trades
};


int main(int argc, char* argv[])
{
	Market market;
	market.AddSymbol()

	return 0;
}

