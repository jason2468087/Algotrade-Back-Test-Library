#pragma once
#include <array>
#include <sstream>
#include "SysType.h"

class TradeLogHandler
{
public:
	TradeLogHandler();

	//===== Log Event =====
	void logBuy(Date d, Symbol sym, Price price, int amount);
	void logSell(Date d, Symbol sym, Price price, int amount);
	void logDividend(Date d, Symbol sym, Price dividend);
	void logSalary(Date d, Price salary);
	void logMessage(Date d, std::string message);

	//===== Save/Clear/Check Log =====
	void saveLog();
	void clearLog();
	bool checkLogExist(Date date, LogType type, int amount, Symbol sym, Price price);

	//===== Get/Set =====
	TradeLogTable getLogTable();
	std::string getContentString();

	//===== Price =====
	void printLog(int tap = 0);
	void printLogLine(int line);
	std::string printLogType(LogType type);

private:
	//===== Data =====
	TradeLogTable logTable{};
	std::ostringstream logStream;
};