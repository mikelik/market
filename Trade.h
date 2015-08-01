#pragma once

#include <chrono>
#include <string>

typedef std::chrono::time_point<std::chrono::system_clock> Timestamp;

#include "Stock.h"
class Trade {
public:
	enum class TradeType { BUY, SELL };
	Trade(std::string aSymbol, unsigned int aQuantity, Price aPrice, TradeType aType,
		Timestamp aTimestamp = std::chrono::system_clock::now()) :
		symbol(aSymbol), quantity(aQuantity), price(aPrice), type(aType), timestamp(aTimestamp) {};

private:
	std::string symbol;
	Timestamp timestamp;
	unsigned int quantity;
	Price price;
	TradeType type;

};
