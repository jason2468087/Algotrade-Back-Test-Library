#pragma once

#include "Portfolio.h"
#include "Share.h"
#include "Market.h"
#include "Broker.h"
#include "Calendar.h"
#include "AssetLogHandler.h"
#include "ProfitLogHandler.h"
#include "TradeLogHandler.h"
#include "ErrorPacket.h"
#include "Index.h"
#include "SysConst.h"

class ControlPanel
{
public:
	void run();

	//===== Trade Operation (User) =====
	void buyStock(Order order);
	void sellStock(Order order);
	void receiveSalary(Price salary);

	void logTradeMessage(std::string msg);
	void printFinalStatus();

	//===== Set Actions (user) =====
	void setInitAction(Action);
	void setDailyOpenAction(Action);
	void setDailyCloseAction(Action);
	void setDailyTwiceAction(Action);
	void setMonthlyAction(Action);
	void setYearlyAction(Action);
	void setMonthlyAction(Action, int day);
	void setYearlyAction(Action, int month, int day);
	void addSingleAction(Action, Date date);

	//===== Get/Set (User) =====
	void setDuration(Date& start, Date& end);
	Date getToday();
	bool getIsMarketOpen();

	//===== Get/Set (Library) =====
	Portfolio& getPortfolioRef();
	TradeLogTable getTradeLog();
	Market& getMarketRef();
	Index& getIndexRef();

private:
	//===== Library Components =====
	Calendar calendar{ rawStartDate };
	Market market{};
	Portfolio portfolio{ market };
	AssetLogHandler assetLogHandler{};
	ProfitLogHandler profitLogHandler{};
	Index index{ market };

	//===== Order Storage =====
	std::vector<Order> pendingBuyOrderArr{};
	std::vector<Order> pendingSellOrderArr{};

	//===== Start/End Dates =====
	Date startDate{};
	Date endDate{};
	RawDate rawStartDate{};
	RawDate rawEndDate{};

	//===== Actions =====
	Action initAction{ nullptr };
	Action dailyOpenAction{ nullptr };
	Action dailyCloseAction{ nullptr };
	Action dailyTwiceAction{ nullptr };
	Action monthlyAction{ nullptr };
	Action yearlyAction{ nullptr };
	std::vector<std::pair<Date, Action>> singleActionPairArr{};

	int monthlyRepeatDay{};
	int yearlyRepeatDay{};
	int yearlyRepeatMonth{};

	//===== Emulation Operations =====
	void updateMarket();
	void deliverDividend();
	void runMainEmulationLoop();
	void takeEmulationAction();
	void logTodayAsset();
	void logTodayProfit();
	void saveAssetLog();

	void handlePendingBuyOrder();
	void handlePendingSellOrder();

	void handleRepeatAction(bool isYearlyRepeatDay, bool isMonthlyRepeatDay);
	void handleSingleAction();
	void postponeMissedAction(bool isYearlyRepeatDay, bool isMonthlyRepeatDay);

	//===== Emulation Flags =====
	bool isMonthlyActionPending{ false };
	bool isYearlyActionPending{ false };
};