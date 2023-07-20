#include "Portfolio.h"
#include "ErrorPacket.h"

#include <iostream>

///////////////////////////////////////////////////////////////////
// Construction
///////////////////////////////////////////////////////////////////

Portfolio::Portfolio(Market& market)
	: marketRef{ market }
{

}

///////////////////////////////////////////////////////////////////
// Updates
///////////////////////////////////////////////////////////////////

void Portfolio::initialUpdate()
{
	shareHoldingList.clear();
	updateInitialStatistic();
}

void Portfolio::finalUpdate()
{
	updateFinalStatistic();
	tradeLogHandler.saveLog();
}

void Portfolio::updateInitialStatistic()
{
	statistic.initialNetWorth = getTotalWorth();
}

void Portfolio::updateFinalStatistic()
{
	statistic.finalNetWorth = getTotalWorth();
	statistic.totalGrowth = statistic.finalNetWorth - statistic.initialNetWorth;
	statistic.totalGrowthPercent = (static_cast<double>(statistic.finalNetWorth) - statistic.initialNetWorth) / statistic.initialNetWorth;
	statistic.priceGrowth = statistic.totalGrowth - statistic.totalDividend - statistic.totalSalary;
}

void Portfolio::updateTodayStatistic()
{
	calculateTotalWorth();
	if (totalWorth > statistic.maxNetWorth)
	{
		statistic.maxNetWorth = totalWorth;
	}

	if (totalWorth < statistic.minNetWorth)
	{
		statistic.minNetWorth = totalWorth;
	}
}

///////////////////////////////////////////////////////////////////
// Portfolio Operations
///////////////////////////////////////////////////////////////////
void Portfolio::buyStock(Symbol symbol, int amount)
{
	marketRef.observe(symbol);

	Price sharePrice = marketRef.getTodayClose(symbol);
	Price purchaseCost = sharePrice * amount + broker.getTradeFee(sharePrice * amount, amount);
	bool isCashEnough = (cash >= purchaseCost);

	if (!isCashEnough)
		throw(ErrorPacket{ ErrorCode::NOT_ENOUGH_CASH,amount,symbol });

	cash -= purchaseCost;

	bool isSharePurchased = shareHoldingList.find(symbol) != shareHoldingList.end();
	if (!isSharePurchased)
	{
		shareHoldingList.emplace(symbol, 0);
	}
	shareHoldingList.at(symbol) += amount;

	statistic.totalPriceBought += purchaseCost;
	statistic.totalStockBought += amount;

	tradeLogHandler.logBuy(today, symbol, sharePrice, amount);
}

void Portfolio::buyStock(Symbol symbol, int amount, Price buyPrice)
{
	marketRef.observe(symbol);

	Price purchaseCost = buyPrice * amount + broker.getTradeFee(buyPrice * amount, amount);
	bool isCashEnough = (cash >= purchaseCost);

	if (!isCashEnough)
		throw(ErrorPacket{ ErrorCode::NOT_ENOUGH_CASH,amount,symbol });

	cash -= purchaseCost;

	bool isSharePurchased = shareHoldingList.find(symbol) != shareHoldingList.end();
	if (!isSharePurchased)
	{
		shareHoldingList.emplace(symbol, 0);
	}
	shareHoldingList.at(symbol) += amount;

	statistic.totalPriceBought += purchaseCost;
	statistic.totalStockBought += amount;

	tradeLogHandler.logBuy(today, symbol, buyPrice, amount);
}

void Portfolio::sellStock(Symbol symbol, int amount)
{
	if (shareHoldingList.find(symbol) == shareHoldingList.end())
		throw(ErrorPacket{ ErrorCode::SELL_UNOWNED_STOCK,symbol });
	else if (amount > shareHoldingList.at(symbol))
		throw(ErrorPacket{ ErrorCode::SELL_AMOUNT_EXCEEDED,amount,symbol });

	shareHoldingList.at(symbol) -= amount;

	Price sharePrice = marketRef.getTodayClose(symbol);
	Price sellingCost = sharePrice * amount + broker.getTradeFee(sharePrice * amount, amount);
	cash += sellingCost;

	statistic.totalPriceSold += sellingCost;
	statistic.totalStockSold += amount;

	tradeLogHandler.logSell(today, symbol, sharePrice, amount);
}

void Portfolio::sellStock(Symbol symbol, int amount, Price sellPrice)
{
	if (shareHoldingList.find(symbol) == shareHoldingList.end())
		throw(ErrorPacket{ ErrorCode::SELL_UNOWNED_STOCK,symbol });
	else if (amount > shareHoldingList.at(symbol))
		throw(ErrorPacket{ ErrorCode::SELL_AMOUNT_EXCEEDED,amount,symbol });

	shareHoldingList.at(symbol) -= amount;

	Price sellingCost = sellPrice * amount + broker.getTradeFee(sellPrice * amount, amount);
	cash += sellingCost;

	statistic.totalPriceSold += sellingCost;
	statistic.totalStockSold += amount;

	tradeLogHandler.logSell(today, symbol, sellPrice, amount);
}

void Portfolio::receiveSalary(Price salary)
{
	if (salary < 0)
		throw(ErrorPacket{ ErrorCode::NEGATIVE_SALARY,salary });

	cash += salary;
	totalWorth += salary;

	tradeLogHandler.logSalary(today, salary);
	statistic.totalSalary += salary;
}

void Portfolio::receiveDividend(Symbol symbol, Price totalDividend)
{
	if (totalDividend < 0)
	{
		throw(ErrorPacket{ ErrorCode::NEGATIVE_DIVIDEND,totalDividend,symbol });
	}

	cash += totalDividend;
	totalWorth += totalDividend;
	if (totalDividend > 0)
	{
		tradeLogHandler.logDividend(today, symbol, totalDividend);
		statistic.totalDividend += totalDividend;
	}
}

///////////////////////////////////////////////////////////////////
// Calculation
///////////////////////////////////////////////////////////////////

void Portfolio::calculateTotalWorth()
{
	totalWorth = 0;

	for (auto& thisShare : shareHoldingList)
	{
		const Symbol& symbol{ thisShare.first };
		const int& amount{ thisShare.second };
		totalWorth += marketRef.getClose(symbol, today) * amount;
	}
	totalWorth += cash;
}

///////////////////////////////////////////////////////////////////
// Print
///////////////////////////////////////////////////////////////////
void Portfolio::printPortfolio()
{
	calculateTotalWorth();
	std::cout << "=====Portfolio Status=====\n";
	std::cout << "Date:" << today << '\n';
	std::cout << "Total Worth: " << ptos(totalWorth) << '\n';
	std::cout << "Cash: " << ptos(cash) << '['<<static_cast<double>(cash) / totalWorth * 100 << "%]" << '\n';
	for (auto& thisShare : shareHoldingList)
	{
		const Symbol& sym{ thisShare.first };
		Price sharePrice{ marketRef.getClose(sym,today) };
		int shareCount{ thisShare.second };
		std::cout << '[' << thisShare.first << "] ";
		std::cout << "Count: " << shareCount << " ";
		std::cout << "Price: " << ptos(sharePrice * shareCount) << " ";
		std::cout << "Percent: " << getStockProportion(sym) * 100 << "%\n";
	}
}

void Portfolio::printStatistic()
{
	std::cout << "=====Statistic=====\n";
	std::cout << "Initial Net Worth: " << ptos(statistic.initialNetWorth) << std::endl;
	std::cout << "Final Net Worth: " << ptos(statistic.finalNetWorth) << std::endl;
	std::cout << "Maximum Net Worth: " << ptos(statistic.maxNetWorth) << std::endl;
	std::cout << "Minimum Net Worth: " << ptos(statistic.minNetWorth) << std::endl;

	std::cout << "Total Price Brought: " << ptos(statistic.totalPriceBought) << std::endl;
	std::cout << "Total Price Sold: " << ptos(statistic.totalPriceSold) << std::endl;
	std::cout << "Total Stock Brought: " << ptos(statistic.totalStockBought) << std::endl;
	std::cout << "Total Stock Sold: " << ptos(statistic.totalStockSold) << std::endl;

	std::cout << "Total Growth: " << ptos(statistic.totalGrowth) << std::endl;
	std::cout << "Total Growth Percent: " << ptos(static_cast<int>(statistic.totalGrowthPercent*100)) << std::endl;
	std::cout << "Total Price Growth: " << ptos(statistic.priceGrowth) << std::endl;
	std::cout << "Total Dividend: " << ptos(statistic.totalDividend) << std::endl;
	std::cout << "Total Salaryt: " << ptos(statistic.totalSalary) << std::endl;

}

///////////////////////////////////////////////////////////////////
// Get/Set (Portfolio Data)
///////////////////////////////////////////////////////////////////

void Portfolio::setToday(Date td)
{
	if (!Calendar::isDateValid(td))
		throw(ErrorPacket{ ErrorCode::INVALID_DATE,td });

	today = td;
}

Price Portfolio::getTotalWorth()
{
	calculateTotalWorth();
	return totalWorth;
}

Price Portfolio::getCash()
{
	return cash;
}

void Portfolio::setCash(Price c)
{
	cash = c;
}

int Portfolio::getShareHolded(Symbol symbol)
{
	bool isPurchased = shareHoldingList.find(symbol) != shareHoldingList.end();

	return isPurchased ? shareHoldingList.at(symbol) : 0;
}

Price Portfolio::getStockWorth(Symbol symbol)
{
	bool isPurchased = shareHoldingList.find(symbol) != shareHoldingList.end();

	Price sharePrice{};
	if (isPurchased)
	{
		sharePrice = marketRef.getTodayClose(symbol);
	}
	return getShareHolded(symbol) * sharePrice;
}

double Portfolio::getStockProportion(Symbol symbol)
{
	bool isPurchased = shareHoldingList.find(symbol) != shareHoldingList.end();

	Price sharePrice{};
	if (isPurchased)
	{
		sharePrice = marketRef.getClose(symbol, today);
	}
	return (double)getShareHolded(symbol) * sharePrice / getTotalWorth();
}

ShareList& Portfolio::getShareHoldingListRef()
{
	return shareHoldingList;
}

int Portfolio::getPortfolioSize()
{
	return static_cast<int>(shareHoldingList.size());
}

///////////////////////////////////////////////////////////////////
// Get/Set (Component)
///////////////////////////////////////////////////////////////////

void Portfolio::setBroker(BrokerName bk)
{
	broker.setBroker(bk);
}

Broker Portfolio::getBroker()
{
	return broker;
}

void Portfolio::setMarketRef(Market& market)
{
	marketRef = market;
}

Statistic& Portfolio::getStatisticRef()
{
	return statistic;
}

TradeLogHandler& Portfolio::getTradeLogHandlerRef()
{
	return tradeLogHandler;
}