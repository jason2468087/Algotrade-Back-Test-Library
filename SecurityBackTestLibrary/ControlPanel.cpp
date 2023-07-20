#include "ControlPanel.h"
#include "ErrorPacket.h"


///////////////////////////////////////////////////////////////////
// Run Emulation
///////////////////////////////////////////////////////////////////
void ControlPanel::run()
{
	market.observe("SPY");

	int runIter{ rawEndDate - rawStartDate };

	updateMarket();
	portfolio.setToday(startDate);
	portfolio.initialUpdate();
	if (initAction != nullptr)
	{
		initAction(portfolio, this, market, index);
	}

	runMainEmulationLoop();

	portfolio.finalUpdate();
	saveAssetLog();
	
}

void ControlPanel::runMainEmulationLoop()
{
	for (calendar.setToday(rawStartDate); calendar.getRawToday() < rawEndDate; ++calendar)
	{
		updateMarket();

		takeEmulationAction();

		deliverDividend();

		portfolio.updateTodayStatistic();

		logTodayAsset();
		logTodayProfit();
	}
}

void ControlPanel::takeEmulationAction()
{
	Date today{ calendar.getToday() };

	bool isMonthlyRepeatDay = today.d == monthlyRepeatDay;
	bool isYearlyRepeatDay = today.m == yearlyRepeatMonth && today.d == yearlyRepeatDay;
	bool isTradeDay = market.getIsTraded("SPY", today);

	if (isTradeDay)
	{
		handlePendingBuyOrder();
		handlePendingSellOrder();

		handleRepeatAction(isYearlyRepeatDay, isMonthlyRepeatDay);
		handleSingleAction();
	}
	else
	{
		postponeMissedAction(isYearlyRepeatDay, isMonthlyRepeatDay);
	}
}

void ControlPanel::handleRepeatAction(bool isYearlyRepeatDay, bool isMonthlyRepeatDay)
{
	market.setIsMarketOpen(true);
	if (yearlyAction != nullptr && (isYearlyRepeatDay || isYearlyActionPending))
	{
		yearlyAction(portfolio, this, market, index);
		isYearlyActionPending = false;
	}
	if (monthlyAction != nullptr && (isMonthlyRepeatDay || isMonthlyActionPending))
	{
		monthlyAction(portfolio, this, market, index);
		isMonthlyActionPending = false;
	}
	if (dailyOpenAction != nullptr)
	{
		dailyOpenAction(portfolio, this, market, index);
	}
	if (dailyTwiceAction != nullptr)
	{
		dailyTwiceAction(portfolio, this, market, index);
	}

	market.setIsMarketOpen(false);
	if (dailyTwiceAction != nullptr)
	{
		dailyTwiceAction(portfolio, this, market, index);
	}
	if (dailyCloseAction != nullptr)
	{
		dailyCloseAction(portfolio, this, market, index);
	}
}

void ControlPanel::handleSingleAction()
{
	for (size_t i = 0; i < singleActionPairArr.size(); i++)
	{
		if (singleActionPairArr[i].first == getToday())
		{
			singleActionPairArr[i].second(portfolio, this, market, index);
			singleActionPairArr.erase(singleActionPairArr.begin() + i);
			i--;
		}
	}
}

void ControlPanel::postponeMissedAction(bool isYearlyRepeatDay, bool isMonthlyRepeatDay)
{
	if (isYearlyRepeatDay) { isYearlyActionPending = true; }
	if (isMonthlyRepeatDay) { isMonthlyActionPending = true; }
}

void ControlPanel::updateMarket()
{
	portfolio.setToday(calendar.getToday());
	market.update(calendar.getRawToday());
}

void ControlPanel::deliverDividend()
{
	std::vector<Price> dividendLogRow{};

	for (auto& pair : portfolio.getShareHoldingListRef())
	{
		Symbol symbol{ pair.first };
		int shareHolded{ pair.second };
		Price totalDividend{ dtop(market.getTodayDividend(symbol) * shareHolded) };

		portfolio.receiveDividend(symbol, totalDividend);
		dividendLogRow.push_back(totalDividend);
	}
	profitLogHandler.logDividend(calendar.getToday(), dividendLogRow);
}

void ControlPanel::logTodayAsset()
{
	ShareList& holdingList{ portfolio.getShareHoldingListRef() };

	assetLogHandler.logTotalWorth(calendar.getToday(), portfolio.getTotalWorth());

	std::vector<Price> worthArr{};
	std::vector<double> ratioArr{};
	for (auto& pair : holdingList)
	{
		Symbol symbol{ pair.first };

		worthArr.push_back(portfolio.getStockWorth(symbol));
		ratioArr.push_back(static_cast<double>(portfolio.getCash() / portfolio.getTotalWorth()));
	}
	worthArr.push_back(portfolio.getCash());
	ratioArr.push_back(static_cast<double>(portfolio.getCash()) / portfolio.getTotalWorth());

	assetLogHandler.logStockWorth(calendar.getToday(), worthArr);
	assetLogHandler.logStockRatio(calendar.getToday(), ratioArr);
}

void ControlPanel::logTodayProfit()
{
	ShareList& holdingList{ portfolio.getShareHoldingListRef() };

	std::unordered_map<Symbol, Price> worthArr{};
	for (auto& pair : holdingList)
	{
		Symbol symbol{ pair.first };
		worthArr.emplace(symbol, portfolio.getStockWorth(symbol));
	}

	profitLogHandler.logCapitalGain(calendar.getToday(), worthArr);
}

void ControlPanel::saveAssetLog()
{
	ShareList& holdingList{ portfolio.getShareHoldingListRef() };

	std::vector<Symbol> symbolArr{};
	for (auto& pair : holdingList)
	{
		symbolArr.push_back(pair.first);
	}
	assetLogHandler.setHeaderSymbolRow(symbolArr);
	profitLogHandler.setHeaderSymbolRow(symbolArr);

	assetLogHandler.saveTotalWorthLog();
	assetLogHandler.saveStockWorthLog();
	assetLogHandler.saveStockRatioLog();

	profitLogHandler.saveCapitalGainLog();
	profitLogHandler.saveFixedIncomeLog();
}

///////////////////////////////////////////////////////////////////
// Set Actions
///////////////////////////////////////////////////////////////////
void ControlPanel::setInitAction(Action initAct)
{
	initAction = initAct;
}

void ControlPanel::setDailyOpenAction(Action dailyAct)
{
	dailyOpenAction = dailyAct;
}

void ControlPanel::setDailyCloseAction(Action dailyAct)
{
	dailyCloseAction = dailyAct;
}

void ControlPanel::setDailyTwiceAction(Action dailyAct)
{
	dailyTwiceAction = dailyAct;
}

void ControlPanel::setMonthlyAction(Action monthAct)
{
	monthlyRepeatDay = 1;
	monthlyAction = monthAct;
}

void ControlPanel::setMonthlyAction(Action monthAct, int day)
{
	monthlyRepeatDay = day;
	monthlyAction = monthAct;
}

void ControlPanel::setYearlyAction(Action yearAct)
{
	yearlyRepeatDay = 1;
	yearlyRepeatMonth = 1;
	yearlyAction = yearAct;
}
void ControlPanel::setYearlyAction(Action yearAct,int month, int day)
{
	yearlyRepeatDay = day;
	yearlyRepeatMonth = month;
	yearlyAction = yearAct;
}

void ControlPanel::addSingleAction(Action singleAct, Date date)
{
	singleActionPairArr.push_back(std::pair<Date, Action>(date, singleAct));
}

///////////////////////////////////////////////////////////////////
// Order
///////////////////////////////////////////////////////////////////

void ControlPanel::buyStock(Order order)
{
	switch (order.orderType)
	{
	case MARKET_ORDER:
		portfolio.buyStock(order.symbol, order.amount, market.getPrice(order.symbol));
		profitLogHandler.updateTradeBalance(order.symbol, order.amount * market.getTodayOpen(order.symbol));
		break;

	default:
		pendingBuyOrderArr.push_back(order);

		if (!Calendar::isDateValid(order.expiryDate))
		{
			throw(ErrorPacket{ErrorCode::INVALID_DATE,order.expiryDate});
		}

		market.observe(order.symbol);

		break;
	}
}

void ControlPanel::sellStock(Order order)
{
	switch (order.orderType)
	{
	case MARKET_ORDER:
		portfolio.sellStock(order.symbol, order.amount, market.getPrice(order.symbol));
		profitLogHandler.updateTradeBalance(order.symbol, -order.amount * market.getTodayOpen(order.symbol));
		break;

	default:
		if (!Calendar::isDateValid(order.expiryDate))
		{
			throw(ErrorPacket{ ErrorCode::INVALID_DATE,order.expiryDate });
		}

		pendingSellOrderArr.push_back(order);

		break;
	}
}

void ControlPanel::handlePendingBuyOrder()
{
	Date today{ calendar.getToday() };
	RawDate rawToday{ calendar.getRawToday() };

	std::vector<int> removeArr{};
	for (size_t i = 0; i < pendingBuyOrderArr.size(); i++)
	{
		Order& order{ pendingBuyOrderArr[i] };
		Symbol symbol{ order.symbol };

		bool isLimitBuyNeeded = order.orderType == LIMIT_ORDER && market.getLow(symbol, today) <= order.price;
		bool isStopBuyNeeded = order.orderType == STOP_ORDER && market.getHigh(symbol, today) >= order.price;
		if (isLimitBuyNeeded || isStopBuyNeeded)
		{
			portfolio.buyStock(order.symbol, order.amount, order.price);
			profitLogHandler.updateTradeBalance(order.symbol, order.amount * market.getTodayOpen(order.symbol));
			removeArr.push_back(static_cast<int>(i));
		}
		bool isExpired = rawToday > Calendar::convertToRawDate(order.expiryDate);
		if (isExpired)
		{
			removeArr.push_back(static_cast<int>(i));
		}
	}

	for (int i = removeArr.size() - 1; i >= 0; i--)
	{
		pendingBuyOrderArr.erase(pendingBuyOrderArr.begin() + removeArr[i]);
	}
}

void ControlPanel::handlePendingSellOrder()
{
	Date today{ calendar.getToday() };

	std::vector<int> removeArr{};
	for (size_t i = 0; i < pendingSellOrderArr.size(); i++)
	{
		Order& order{ pendingSellOrderArr[i] };
		Symbol symbol{ order.symbol };

		bool isLimitSellNeeded = order.orderType == LIMIT_ORDER && market.getHigh(symbol, getToday()) >= order.price;
		bool isStopSellNeeded = order.orderType == STOP_ORDER && market.getLow(symbol, getToday()) <= order.price;
		if (isLimitSellNeeded || isStopSellNeeded)
		{
			portfolio.sellStock(order.symbol, order.amount, order.price);
			profitLogHandler.updateTradeBalance(order.symbol, -order.amount * market.getTodayOpen(order.symbol));
			removeArr.push_back(static_cast<int>(i));
		}
		bool isExpired = Calendar::convertToRawDate(today) > Calendar::convertToRawDate(order.expiryDate);
		if (isExpired)
		{
			removeArr.push_back(static_cast<int>(i));
		}
	}

	for (int i = (int)removeArr.size() - 1; i >= 0; i--)
	{
		pendingSellOrderArr.erase(pendingSellOrderArr.begin() + removeArr[i]);
	}
}

void ControlPanel::logTradeMessage(std::string msg)
{
	portfolio.getTradeLogHandlerRef().logMessage(calendar.getToday(), msg);
}

void ControlPanel::receiveSalary(Price salary)
{
	portfolio.receiveSalary(salary);
	profitLogHandler.logSalary(calendar.getToday(), salary);
}

///////////////////////////////////////////////////////////////////
// Get/Set
///////////////////////////////////////////////////////////////////
void ControlPanel::setDuration(Date& start, Date& end)
{
	if (!Calendar::isDateValid(start))
		throw(ErrorPacket{ ErrorCode::INVALID_DATE,start });
	if (!Calendar::isDateValid(end))
		throw(ErrorPacket{ ErrorCode::INVALID_DATE,end });

	startDate = start;
	endDate = end;
	rawStartDate = Calendar::convertToRawDate(start);
	rawEndDate = Calendar::convertToRawDate(end);
	calendar.setToday(rawStartDate);
}

Date ControlPanel::getToday()
{
	return calendar.getToday();
}

bool ControlPanel::getIsMarketOpen()
{
	return false;
}

Portfolio& ControlPanel::getPortfolioRef()
{
	return portfolio;
}

Market& ControlPanel::getMarketRef()
{
	return market;
}

TradeLogTable ControlPanel::getTradeLog()
{
	return portfolio.getTradeLogHandlerRef().getLogTable();
}

Index& ControlPanel::getIndexRef()
{
	return index;
}

///////////////////////////////////////////////////////////////////
// Print
///////////////////////////////////////////////////////////////////
//#include <iomanip>
void ControlPanel::printFinalStatus()
{
	Price initialWealth{ portfolio.getStatisticRef().initialNetWorth };

	std::cout << "=====Final Result=====\n";
	std::cout << "Duration: " << Calendar::convertToDate(rawStartDate) << " => "
		<< Calendar::convertToDate(rawEndDate) << '\n';
	std::cout << "Wealth growth: $" << initialWealth << " => $" << portfolio.getTotalWorth()
		<< '(' << (portfolio.getTotalWorth() - initialWealth) / initialWealth * 100.0 << "%)";
}