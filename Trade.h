#pragma once

#include <chrono>
#include <string>

typedef std::chrono::time_point<std::chrono::system_clock> Timestamp;

#include "Stock.h"
class Trade {
public:
	enum class TradeType { BUY, SELL };
	enum class LongShortType { LONG, SHORT };
	Trade(const std::string& aSymbol, unsigned int aQuantity, Price aPrice, TradeType aType,
		Timestamp aTimestamp = std::chrono::system_clock::now()) :
		symbol(aSymbol), quantity(aQuantity), price(aPrice), type(aType), timestamp(aTimestamp) {};

	Price GetPrice() const { return price; }
	unsigned int GetQuantity() const { return quantity; }
	const Timestamp& GetTimestamp() const { return timestamp; }

private:
	std::string symbol;
	unsigned int quantity;
	Price price;
	TradeType type;
	Timestamp timestamp;
	LongShortType longShortType = LongShortType::LONG; //for simplicity

};
