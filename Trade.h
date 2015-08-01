#pragma once

#include <chrono>
#include "Stock.h"

class Trade {
public:
	enum class TradeType { BUY, SELL };

private:
	string symbol;
	std::chrono::time_point<std::chrono::system_clock> timestamp;
	unsigned int quantity;
	Price price;
	TradeType type;

};
