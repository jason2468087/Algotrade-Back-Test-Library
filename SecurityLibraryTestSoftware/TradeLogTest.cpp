#include "TradeLogTest.h"

#include <fstream>

bool TradeLogTest::test()
{
	std::cout << "========Test Trade Log Class=======\n";
	bool isPass{ true };

	isPass &= testLogTable();
	isPass &= testSaveLog();


	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool TradeLogTest::testLogTable()
{
	bool isPass{ false };
	std::cout << "[Test Log Table]:\n";

	TradeLogHandler tradeLogHandler{};
	tradeLogHandler.logBuy(Date{ 21,7,2004 }, "MSFT", 60.0_USD, 5);
	tradeLogHandler.logSell(Date{ 5,8,2012 }, "IYW", 110.5_USD, 1);
	tradeLogHandler.logDividend(Date{ 30,11,2019 }, "BND", 1614.31_USD);
	tradeLogHandler.logSalary(Date{ 1,4,2018 },2000.0_USD);
	tradeLogHandler.logMessage(Date{ 16,3,2007 }, "This is custom message.");

	TradeLogTable logTable{ tradeLogHandler.getLogTable() };
	isPass = tradeLogHandler.checkLogExist(Date{ 21,7,2004 }, LogType::BUY, 5, "MSFT", 60.0_USD)
		&& tradeLogHandler.checkLogExist(Date{ 5,8,2012 }, LogType::SELL, 1, "IYW", 110.5_USD)
		&& tradeLogHandler.checkLogExist(Date{ 30,11,2019 }, LogType::DIVIDEND, 0, "BND", 1614.31_USD)
		&& tradeLogHandler.checkLogExist(Date{ 1,4,2018 }, LogType::SALARY, 0, "", 2000.0_USD)
		&& tradeLogHandler.checkLogExist(Date{ 16,3,2007 }, LogType::MESSAGE, 0, "This is custom message.", 0);

	tradeLogHandler.printLog(1);
	std::cout << "\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool TradeLogTest::testSaveLog()
{
	bool isPass{ false };
	std::cout << "[Test Saving Log File]:\n";

	TradeLogHandler tradeLogHandler{};
	tradeLogHandler.logBuy(Date{ 21,7,2004 }, "MSFT", 60.0_USD, 5);
	tradeLogHandler.logSell(Date{ 5,8,2012 }, "IYW", 110.5_USD, 1);
	tradeLogHandler.logDividend(Date{ 30,11,2019 }, "BND", 1614.31_USD);
	tradeLogHandler.logSalary(Date{ 1,4,2018 }, 2000.0_USD);
	tradeLogHandler.logMessage(Date{ 16,3,2007 }, "This is custom message.");

	tradeLogHandler.saveLog();

	std::string ans{ R"(21/7/2004 Buy 5 MSFT in $60.00
5/8/2012 Sell 1 IYW in $110.50
30/11/2019 Dividend  BND of $1614.31
1/4/2018 Salary $2000.00
16/3/2007 This is custom message.
)" };

	std::ifstream file{};
	file.open("../SecurityBackTestLibrary/Log/trade_log.txt");
	if (file.is_open())
	{
		std::string str{std::istreambuf_iterator<char>(file),std::istreambuf_iterator <char>()};
		if (str == ans)
		{
			isPass = true;
		}
	}
	std::cout << "\tSaved content correct? " << isPass << " \n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}