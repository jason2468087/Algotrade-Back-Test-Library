#pragma once

#include "Share.h"
#include "Broker.h"
#include "Calendar.h"
#include "TradeLogHandler.h"
#include "Market.h"

#include <unordered_map>
#include <string>

class Portfolio
{
public:
	Portfolio(Market& marketRef);

	//===== Data Update =====
	void initialUpdate();
	void finalUpdate();

	void updateInitialStatistic();
	void updateFinalStatistic();
	void updateTodayStatistic();

	//===== Portfolio Operations =====
	void buyStock(Symbol symbol, int amount);
	void buyStock(Symbol symbol, int amount, Price buyPrice);
	void sellStock(Symbol symbol, int amount);
	void sellStock(Symbol symbol, int amount, Price sellPrice);
	void receiveSalary(Price salary);
	void receiveDividend(Symbol symbol, Price dividend);

	//===== Get/Set Component =====
	void setMarketRef(Market& marketRef);
	void setBroker(BrokerName bk);

	Statistic& getStatisticRef();
	TradeLogHandler& getTradeLogHandlerRef();
	ShareList& getShareHoldingListRef();
	Broker getBroker();

	//===== Get/Set Portfolio Data =====
	void setToday(Date td);
	void setCash(Price c);

	Price getTotalWorth();
	Price getCash();
	int getPortfolioSize();
	int getShareHolded(Symbol symbol);
	int getStockWorth(Symbol symbol);
	double getStockProportion(Symbol symbol);

	//===== Portfolio =====
	void printPortfolio();
	void printStatistic();

private:
	//===== Data =====
	Date today{};
	Price totalWorth{};
	Price cash{ 100000.0_USD };
	ShareList shareHoldingList{};

	//===== Component =====
	Broker broker{ BrokerName::NONE };
	Statistic statistic{};
	TradeLogHandler tradeLogHandler{};
	Market& marketRef;

	//===== Calculation =====
	void calculateTotalWorth();
};