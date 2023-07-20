#include "ControlPanelTest.h"
#include <iomanip>
#include <string>

///////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////

bool ControlPanelTest::test()
{
	std::cout << "========Test Control Panel Class=======\n";
	bool isPass{ true };

	isPass &= testRepeatEvent();
	isPass &= testSingleEvent();
	isPass &= testReceiveDividend();
	isPass &= testRunTrade();
	isPass &= testOrder();


	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Test Repeated Event
///////////////////////////////////////////////////////////////////
int dayCount{};
int monthCount{};
int yearCount{};

void ControlPanelTest::dailyTimeFlowAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	dayCount++;
}

void ControlPanelTest::monthTimeFlowAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	monthCount++;
}

void ControlPanelTest::yearTimeFlowAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	yearCount++;
}

bool ControlPanelTest::testRepeatEvent()
{
	std::cout << "[Test Repeat Event]:\n";
	bool isPass{ true };

	isPass &= testDefaultRepeatEvent();
	isPass &= testFixedDateRepeatEvent();

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ControlPanelTest::testDefaultRepeatEvent()
{
	bool isPass{ false };

	dayCount = 0;
	monthCount = 0;
	yearCount = 0;

	ControlPanel backTestPanel{};

	Date start{ 1,1,2010 };
	Date end{ 1,3,2015 };
	backTestPanel.setDuration(start, end);

	backTestPanel.setDailyOpenAction(dailyTimeFlowAction);
	backTestPanel.setMonthlyAction(monthTimeFlowAction);
	backTestPanel.setYearlyAction(yearTimeFlowAction);

	backTestPanel.run();

	std::cout << "\tDaily Open Event Count:" << dayCount
		<< " Monthly Event Count:" << monthCount
		<< " Yearly Event Count:" << yearCount << "\n";

	bool isCorrect = dayCount == 1297 && monthCount == 62 && yearCount == 6;
	if (isCorrect)
	{
		isPass = true;
	}

	return isPass;
}

bool ControlPanelTest::testFixedDateRepeatEvent()
{
	bool isPass{ false };

	dayCount = 0;
	monthCount = 0;
	yearCount = 0;

	ControlPanel backTestPanel{};

	Date start{ 1,1,2010 };
	Date end{ 18,3,2015 };
	backTestPanel.setDuration(start, end);

	backTestPanel.setDailyOpenAction(dailyTimeFlowAction);
	backTestPanel.setMonthlyAction(monthTimeFlowAction,15);
	backTestPanel.setYearlyAction(yearTimeFlowAction,3,25);
	backTestPanel.run();

	std::cout << "\tDaily Open Event Count:" << dayCount
		<< " Monthly Event Count:" << monthCount
		<< " Yearly Event Count:" << yearCount << "\n";

	bool isCorrect = dayCount == 1309 && monthCount == 63 && yearCount == 5;
	if (isCorrect)
	{
		isPass = true;
	}

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Test Single Event
///////////////////////////////////////////////////////////////////

void ControlPanelTest::singleAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	controlPanel->buyStock(Order{ MARKET_ORDER,"SPY",3 });
}

bool ControlPanelTest::testSingleEvent()
{
	std::cout << "[Test Single Event]:\n";
	bool isPass{ false };

	ControlPanel backTestPanel{};

	Date start{ 1,1,2010 };
	Date end{ 16,9,2010 };
	backTestPanel.setDuration(start, end);

	backTestPanel.addSingleAction(singleAction, Date{ 3,5,2010 });
	backTestPanel.addSingleAction(singleAction, Date{ 14,9,2010 });
	backTestPanel.addSingleAction(singleAction, Date{ 26,4,2010 });
	backTestPanel.run();

	TradeLogHandler& tradeLogHandler{ backTestPanel.getPortfolioRef().getTradeLogHandlerRef() };
	TradeLogTable logTable{ tradeLogHandler.getLogTable() };

	isPass = tradeLogHandler.checkLogExist(Date{ 26,4,2010 }, LogType::BUY, 3, "SPY", 121.35_USD)
		&& tradeLogHandler.checkLogExist(Date{ 3,5,2010 }, LogType::BUY, 3, "SPY", 120.35_USD)
		&& tradeLogHandler.checkLogExist(Date{ 14,9,2010 }, LogType::BUY, 3, "SPY", 112.65_USD);

	for (int r = 0; r < 4; r++)
	{
		std::cout << '\t';
		backTestPanel.getPortfolioRef().getTradeLogHandlerRef().printLogLine(r);
	}

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Receive Dividend
///////////////////////////////////////////////////////////////////

void ControlPanelTest::initDividendAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	market.observe("BND");
	controlPanel->buyStock(Order{ MARKET_ORDER,"BND",100 });
}

bool ControlPanelTest::testReceiveDividend()
{
	std::cout << "[Test Receive Dividend]:\n";
	bool isPass{ false };

	ControlPanel backTestPanel{};

	Date start{ 1,5,2007 };
	Date end{ 30,6,2021 };
	backTestPanel.setDuration(start, end);

	backTestPanel.setInitAction(initDividendAction);

	backTestPanel.run();

	Statistic& statistic{ backTestPanel.getPortfolioRef().getStatisticRef() };
	Price totalDividend{ statistic.totalDividend };

	isPass = totalDividend == 3568.2_USD;

	std::cout << "\tTotal Dividend: " << ptos(totalDividend) << "(3568.2)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Run Trade
///////////////////////////////////////////////////////////////////

void ControlPanelTest::initTradeAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	market.observe("MSFT");
}

void ControlPanelTest::monthTradeAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	controlPanel->buyStock(Order{ MARKET_ORDER,"MSFT",3 });
	portfolio.receiveSalary(500_USD);
}

bool ControlPanelTest::testRunTrade()
{
	std::cout << "[Test Run Trade]:\n";
	bool isPass{ false };

	ControlPanel backTestPanel{};

	Date start{ 17,5,2011 };
	Date end{ 23,3,2013 };
	backTestPanel.setDuration(start, end);

	backTestPanel.setInitAction(initTradeAction);
	backTestPanel.setMonthlyAction(monthTradeAction);

	backTestPanel.run();

	double totalPurchaseStock = backTestPanel.getPortfolioRef().getStatisticRef().totalStockBought;
	double totalPurchasePrice = backTestPanel.getPortfolioRef().getStatisticRef().totalPriceBought;
	double totalSalary = backTestPanel.getPortfolioRef().getStatisticRef().totalSalary;
	double finalWorth = backTestPanel.getPortfolioRef().getTotalWorth();

	isPass = finalWorth == 111053.88_USD;

	std::cout << "\tTotal Purchase Stock:" << backTestPanel.getPortfolioRef().getStatisticRef().totalStockBought << " (66)\n";
	std::cout << "\tTotal Purchase Price:" << backTestPanel.getPortfolioRef().getStatisticRef().totalPriceBought << " ($1864.17)\n";
	std::cout << "\tTotal Salary:" << ptos(backTestPanel.getPortfolioRef().getStatisticRef().totalSalary) << " ($11000)\n";
	std::cout << "\tFinal Total Worth:" << ptos(backTestPanel.getPortfolioRef().getTotalWorth()) << " ($111053.88)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Handle Limit & Stop Order
///////////////////////////////////////////////////////////////////
bool ControlPanelTest::testOrder()
{
	std::cout << "[Test Order]:\n";
	bool isPass{ true };

	ControlPanel backTestPanel{};

	isPass &= testLimitBuyOrder();
	isPass &= testStopBuyOrder();
	isPass &= testLimitSellOrder();
	isPass &= testStopSellOrder();

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

void ControlPanelTest::initLimitBuyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	controlPanel->buyStock(Order{ LIMIT_ORDER,"HSBC",20,122_USD,Date{31,3,2010} });
	controlPanel->buyStock(Order{ LIMIT_ORDER,"HSBC",20,80_USD,Date{31,3,2010} });
	controlPanel->buyStock(Order{ LIMIT_ORDER,"HSBC",20,50_USD,Date{31,3,2010} });

	controlPanel->buyStock(Order{ LIMIT_ORDER,"HSBC",20,10_USD,Date{31,3,2010} });
	controlPanel->buyStock(Order{ LIMIT_ORDER,"HSBC",20,95_USD,Date{15,7,2008} });
}

bool ControlPanelTest::testLimitBuyOrder()
{
	bool isPass{ false };

	ControlPanel backTestPanel{};

	Date start{ 11,6,2008 };
	Date end{ 31,3,2009 };
	backTestPanel.setDuration(start, end);
	backTestPanel.setInitAction(initLimitBuyAction);
	backTestPanel.run();

	TradeLogTable tradeLog{ backTestPanel.getTradeLog() };
	TradeLogHandler& tradeLogHandler{ backTestPanel.getPortfolioRef().getTradeLogHandlerRef() };

	bool isCase1Pass = tradeLogHandler.checkLogExist(Date{ 27,6,2008 }, LogType::BUY, 20, "HSBC", 122_USD);
	bool isCase2Pass = tradeLogHandler.checkLogExist(Date{ 27,10,2008 }, LogType::BUY, 20, "HSBC", 80_USD);
	bool isCase3Pass = tradeLogHandler.checkLogExist(Date{ 3,3,2009 }, LogType::BUY, 20, "HSBC", 50_USD);

	bool isCase4Pass{ true };
	bool isCase5Pass{ true };

	for (size_t i = 0; i < tradeLog.size(); i++)
	{
		auto t = tradeLog[i];
		LogType logType{ std::get<1>(t) };
		Price logPrice{ std::get<4>(t) };
		if (logType == LogType::BUY && logPrice == 15_USD)
		{
			isCase4Pass = false;
		}
		if (logType == LogType::BUY && logPrice == 95_USD)
		{
			isCase5Pass = false;
		}
	}

	isPass = isCase1Pass && isCase2Pass && isCase3Pass && isCase4Pass && isCase5Pass;

	std::cout << "\tLimit Buy Order: [" << isPass << "]\n";
	std::cout << "\t27/6/2008 Buy 20 HSBC in $122.00 [" << isCase1Pass << "]\n";
	std::cout << "\t27/10/2008 Buy 20 HSBC in $80.00 [" << isCase2Pass << "]\n";
	std::cout << "\t3/3/2009 Buy 20 HSBC in $50.00 [" << isCase3Pass << "]\n";
	std::cout << "\tNo Buy 20 HSBC in $10.00 [" << isCase4Pass << "]\n";
	std::cout << "\tNo Buy 20 HSBC in $95.00 [" << isCase5Pass << "]\n";
	std::cout << "\n";

	backTestPanel.getPortfolioRef().getTradeLogHandlerRef().clearLog();

	return isPass;
}

void ControlPanelTest::initStopBuyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	controlPanel->buyStock(Order{ STOP_ORDER,"MSFT",30,65_USD,Date{30,5,2020} });
	controlPanel->buyStock(Order{ STOP_ORDER,"MSFT",20,97_USD,Date{30,5,2020} });
	controlPanel->buyStock(Order{ STOP_ORDER,"MSFT",10,173_USD,Date{30,5,2020} });

	controlPanel->buyStock(Order{ STOP_ORDER,"MSFT",10,405_USD,Date{30,5,2020} });
	controlPanel->buyStock(Order{ STOP_ORDER,"MSFT",10,102_USD,Date{15,2,2018} });
}

bool ControlPanelTest::testStopBuyOrder()
{
	bool isPass{ false };


	ControlPanel backTestPanel{};

	Date start{ 1,9,2016 };
	Date end{ 30,5,2020 };
	backTestPanel.setDuration(start, end);
	backTestPanel.setInitAction(initStopBuyAction);
	backTestPanel.run();

	TradeLogTable tradeLog{ backTestPanel.getTradeLog() };
	TradeLogHandler& tradeLogHandler{ backTestPanel.getPortfolioRef().getTradeLogHandlerRef() };

	bool isCase1Pass = tradeLogHandler.checkLogExist(Date{ 27,1,2017 }, LogType::BUY, 30, "MSFT", 65_USD);
	bool isCase2Pass = tradeLogHandler.checkLogExist(Date{ 12,3,2018 }, LogType::BUY, 20, "MSFT", 97_USD);
	bool isCase3Pass = tradeLogHandler.checkLogExist(Date{ 30,1,2020 }, LogType::BUY, 10, "MSFT", 173_USD);

	bool isCase4Pass{ true };
	bool isCase5Pass{ true };

	for (size_t i = 0; i < tradeLog.size(); i++)
	{
		auto t = tradeLog[i];
		LogType logType{ std::get<1>(t) };
		Price logPrice{ std::get<4>(t) };
		if (logType == LogType::BUY && logPrice == 405_USD)
		{
			isCase4Pass = false;
		}
		if (logType == LogType::BUY && logPrice == 102_USD)
		{
			isCase5Pass = false;
		}
	}

	isPass = isCase1Pass && isCase2Pass && isCase3Pass && isCase4Pass && isCase5Pass;

	std::cout << "\tStop Buy Order: [" << isPass << "]\n";
	std::cout << "\t27/1/2017 Buy 30 MSFT in $65.00 [" << isCase1Pass << "]\n";
	std::cout << "\t12/3/2018 Buy 20 MSFT in $97.00 [" << isCase2Pass << "]\n";
	std::cout << "\t30/1/2020 Buy 10 MSFT in $173.00 [" << isCase3Pass << "]\n";
	std::cout << "\tNo Buy 10 HSBC in $405.00 [" << isCase4Pass << "]\n";
	std::cout << "\tNo Buy 10 HSBC in $102.00 [" << isCase5Pass << "]\n";
	std::cout << "\n";

	backTestPanel.getPortfolioRef().getTradeLogHandlerRef().clearLog();

	return isPass;
}

void ControlPanelTest::initLimitSellAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	market.observe("MSFT");
	controlPanel->buyStock(Order{ MARKET_ORDER,"MSFT",200 });

	controlPanel->sellStock(Order{ LIMIT_ORDER,"MSFT",20,78_USD,Date{30,5,2020} });
	controlPanel->sellStock(Order{ LIMIT_ORDER,"MSFT",20,116_USD,Date{30,5,2020} });
	controlPanel->sellStock(Order{ LIMIT_ORDER,"MSFT",20,171_USD,Date{30,5,2020} });

	controlPanel->sellStock(Order{ LIMIT_ORDER,"MSFT",20,340_USD,Date{30,5,2020} });
	controlPanel->sellStock(Order{ LIMIT_ORDER,"MSFT",20,94_USD,Date{1,1,2018} });
}

bool ControlPanelTest::testLimitSellOrder()
{
	bool isPass{ false };

	ControlPanel backTestPanel{};

	Date start{ 1,9,2016 };
	Date end{ 30,5,2020 };
	backTestPanel.setDuration(start, end);
	backTestPanel.setInitAction(initLimitSellAction);
	backTestPanel.run();

	TradeLogTable tradeLog{ backTestPanel.getTradeLog() };
	TradeLogHandler& tradeLogHandler{ backTestPanel.getPortfolioRef().getTradeLogHandlerRef() };

	bool isCase1Pass = tradeLogHandler.checkLogExist(Date{ 20,10,2017 }, LogType::SELL, 20, "MSFT", 78_USD);
	bool isCase2Pass = tradeLogHandler.checkLogExist(Date{ 3,10,2018 }, LogType::SELL, 20, "MSFT", 116_USD);
	bool isCase3Pass = tradeLogHandler.checkLogExist(Date{ 30,1,2020 }, LogType::SELL, 20, "MSFT", 171_USD);

	bool isCase4Pass{ true };
	bool isCase5Pass{ true };

	for (size_t i = 0; i < tradeLog.size(); i++)
	{
		auto t = tradeLog[i];
		LogType logType{ std::get<1>(t) };
		Price logPrice{ std::get<4>(t) };
		if (logType == LogType::SELL && logPrice == 340_USD)
		{
			isCase4Pass = false;
		}
		if (logType == LogType::SELL && logPrice == 94_USD)
		{
			isCase5Pass = false;
		}
	}

	isPass = isCase1Pass && isCase2Pass && isCase3Pass && isCase4Pass && isCase5Pass;

	std::cout << "\tLimit Sell Order: [" << isPass << "]\n";
	std::cout << "\t20/10/2017 Sell 20 MSFT in $78.00 [" << isCase1Pass << "]\n";
	std::cout << "\t3/10/2018 Sell 20 MSFT in $116.00 [" << isCase2Pass << "]\n";
	std::cout << "\t30/1/2020 Sell 20 MSFT in $171.00 [" << isCase3Pass << "]\n";
	std::cout << "\tNo Sell 20 MSFT in $340.00 [" << isCase4Pass << "]\n";
	std::cout << "\tNo Sell 20 MSFT in $94.00 [" << isCase5Pass << "]\n";
	std::cout << "\n";

	backTestPanel.getPortfolioRef().getTradeLogHandlerRef().clearLog();

	return isPass;
}

void ControlPanelTest::initStopSellAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
	market.observe("HSBC");
	controlPanel->buyStock(Order{ MARKET_ORDER,"HSBC",200 });

	controlPanel->sellStock(Order{ STOP_ORDER,"HSBC",20,118_USD,Date{31,3,2009} });
	controlPanel->sellStock(Order{ STOP_ORDER,"HSBC",20,84_USD,Date{31,3,2009} });
	controlPanel->sellStock(Order{ STOP_ORDER,"HSBC",20,48_USD,Date{31,3,2009} });

	controlPanel->sellStock(Order{ STOP_ORDER,"HSBC",20,23_USD,Date{31,3,2009} });
	controlPanel->sellStock(Order{ STOP_ORDER,"HSBC",20,100_USD,Date{1,7,2008} });
}

bool ControlPanelTest::testStopSellOrder()
{
	bool isPass{ false };

	ControlPanel backTestPanel{};

	Date start{ 1,6,2008 };
	Date end{ 31,3,2009 };
	backTestPanel.setDuration(start, end);
	backTestPanel.setInitAction(initStopSellAction);
	backTestPanel.run();

	TradeLogTable tradeLog{ backTestPanel.getTradeLog() };
	TradeLogHandler& tradeLogHandler{ backTestPanel.getPortfolioRef().getTradeLogHandlerRef() };

	bool isCase1Pass = tradeLogHandler.checkLogExist(Date{ 2,7,2008 }, LogType::SELL, 20, "HSBC", 118_USD);
	bool isCase2Pass = tradeLogHandler.checkLogExist(Date{ 27,10,2008 }, LogType::SELL, 20, "HSBC", 84_USD);
	bool isCase3Pass = tradeLogHandler.checkLogExist(Date{ 3,3,2009 }, LogType::SELL, 20, "HSBC", 48_USD);

	bool isCase4Pass{ true };
	bool isCase5Pass{ true };

	for (size_t i = 0; i < tradeLog.size(); i++)
	{
		auto t = tradeLog[i];
		LogType logType{ std::get<1>(t) };
		Price logPrice{ std::get<4>(t) };
		if (logType == LogType::SELL && logPrice == 23_USD)
		{
			isCase4Pass = false;
		}
		if (logType == LogType::SELL && logPrice == 100_USD)
		{
			isCase5Pass = false;
		}
	}

	isPass = isCase1Pass && isCase2Pass && isCase3Pass && isCase4Pass && isCase5Pass;

	std::cout << "\tStop Sell Order: [" << isPass << "]\n";
	std::cout << "\t2/7/2008 Sell 20 HSBC in $118.00 [" << isCase1Pass << "]\n";
	std::cout << "\t27/10/2008 Sell 20 HSBC in $84.00 [" << isCase2Pass << "]\n";
	std::cout << "\t3/3/2009 Sell 20 HSBC in $48.00 [" << isCase3Pass << "]\n";
	std::cout << "\tNo Sell 20 HSBC in $23.00 [" << isCase4Pass << "]\n";
	std::cout << "\tNo Sell 20 HSBC in $100.00 [" << isCase5Pass << "]\n";
	std::cout << "\n";

	backTestPanel.getPortfolioRef().getTradeLogHandlerRef().clearLog();

	return isPass;
}